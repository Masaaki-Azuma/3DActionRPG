#include "Player.h"

#include "Util/Input.h"
#include "Util/DxConverter.h"
#include "Util/Matrix4x4.h"
#include "Util/MyMath.h"
#include "AssetsManager/Mesh.h"
#include "AssetsManager/EnemyDatabase.h"
#include "BattleScene/IWorld.h"
#include "AttackCollider.h"

const float Speed{ 10.0f };
const float AvoidSpeed{ 15.0f };
const Vector3 CollisionOffset{ 0, 50.0f, 0 };
const float CollisionRadius{ 50.0f };

//モーション番号
/*名前の衝突回避のために、無名列挙型には接頭語をつけることを推奨する*/
enum
{
	Motion_Attack01 = 0,
	Motion_Attack02 = 1,
	Motion_Attack03 = 2,
	Motion_Attack04 = 3,
	Motion_Attack05 = 4,
	Motion_Die = 9,
	Motion_Recover = 10,
	Motion_Damage = 14,
	Motion_Idle = 15,
	Motion_RollingForward = 22,
	Motion_RollingBackward = 23,
	Motion_RunForward = 26,
	Motion_RunBackward = 27,
	Motion_Sprint = 29,
};

Player::Player(IWorld* world):
	mesh_{Mesh::player_handle, 30.0f},
	state_{State::Move},
	state_timer_{0.0f},
	motion_{Motion_Idle},
	motion_loop_{true}
{
	world_ = world;
	name_ = "Player";
	tag_ = "PlayerTag";
	position_ = Vector3{ 0.0f, 0.0f, 0.0f };
	collider_ = Sphere{ CollisionRadius, CollisionOffset };
	parameter_ = p_db_.get_current_parameter();

	//メッシュ姿勢初期化
	mesh_.change_anim(motion_, motion_loop_);
	mesh_.set_position(position_);
	mesh_.set_rotation(rotation_* MyMath::Deg2Rad);
}

Player::‾Player()
{
	p_db_.set_hp(parameter_.hp);
}

void Player::update(float delta_time)
{
	
	update_state(delta_time);
	//状態タイマーの更新
	update_state_timer_(delta_time);
	//壁の押し出し処理
	react_wall();

	//メッシュの更新
	mesh_.change_anim(motion_, motion_loop_, motion_interruption_);
	mesh_.set_position(position_);
	mesh_.set_rotation(rotation_ * MyMath::Deg2Rad);
	mesh_.update(delta_time);
}

void Player::draw() const
{
	//ForDebug
	collider().draw();
	
	//メッシュを描画
	mesh_.draw();

	//ForDebug
	//DxLib::DrawFormatString(0, 0, DxLib::GetColor(255, 255, 255), "motion:%d", motion_);
	DxLib::DrawFormatString(0, 0, DxLib::GetColor(255, 255, 255), "player_hp:%d", parameter_.hp);

}

void Player::react(Actor& other)
{
	if (other.tag() == "EnemyAttackTag") {
		int damage = EnemyDatabase::GetInstance().get_attack(other.name());
		take_damage(damage);
		if (parameter_.hp <= 0) {
			enable_collider_ = false;
			parameter_.hp = 0;
			change_state(State::Dead, Motion_Die, false);
			mesh_.change_anim(Motion_Die, motion_loop_);
			return;
		}
		change_state(State::Damage, Motion_Damage, false, true);
		mesh_.change_anim(Motion_Die, motion_loop_, motion_interruption_);
	}

	if (other.tag() == "EnemyTag") {
		collide_actor(other);
	}
}

bool Player::store_model_handle(int model_handle)
{
	mesh_.store_model_handle(model_handle);
	return model_handle != -1;
}

void Player::update_state(float delta_time)
{
	motion_interruption_ = false;
	//状態による行動分岐
	switch (state_) {
	case State::Move:   move(delta_time);   break;
	case State::Attack: attack(delta_time); break;
	case State::Damage: damage(delta_time); break;
	case State::Avoid:  avoid(delta_time);  break;
	case State::Dead:   dead(delta_time);   break;
	}
}

//状態とモーションを変化、ループのないモーション設定時はは必ず呼ぶ
void Player::change_state(State state, unsigned int motion, bool loop, bool motion_interruption)
{
	state_ = state;
	motion_ = motion;
	motion_loop_ = loop;
	motion_interruption_ = motion_interruption;
	state_timer_ = 0.0f;
}

void Player::move(float delta_time)
{
	//回避
	if (Input::get_button_down(PAD_INPUT_2)) {
		change_state(State::Avoid, Motion_RollingForward, false);
		avoid(delta_time);
		sound_.play_SE(SE_Avoid);
		return;
	}

	//攻撃
	if (Input::get_button_down(PAD_INPUT_1)) {
		change_state(State::Attack, Motion_Attack01, false);
		return;
	}

	//移動状態のモーション番号
	unsigned int motion = Motion_Idle;
	//カメラの前方向を取得
	Vector3 camera_rotation = camera_angle();
	Vector3 forward = camera_forward();
	//移動
	velocity_ = Vector3::ZERO;
	if (Input::get_button(PAD_INPUT_UP)) {
		velocity_ = forward * Speed;
		rotation_.y = camera_rotation.y;
		motion = Motion_Sprint;
	}
	else if (Input::get_button(PAD_INPUT_DOWN)) {
		velocity_ = -forward * Speed;
		rotation_.y = camera_rotation.y;
		motion = Motion_RunBackward;
	}
	position_ += velocity_;
	change_state(State::Move, motion);
}

void Player::attack(float delta_time)
{
	//コンボ攻撃予定か？
	static bool is_combo = false;
	//コンボ攻撃回数
	static int combo_counter = 0;

	//回避ボタンを押すと割り込んで回避状態へ
	if (Input::get_button_down(PAD_INPUT_2)) {
		//攻撃状態を初期化
		is_combo = false;
		combo_counter = 0;
		change_state(State::Avoid, Motion_RollingForward, false);
		return;
	}

	//各攻撃モーションの適切なタイミングで攻撃判定を生成
	switch (motion_) {
	case Motion_Attack01: timely_generate_attack(0.2f); break;
	case Motion_Attack02: timely_generate_attack(0.2f); break;
	case Motion_Attack03: timely_generate_attack(0.15f); break;
	case Motion_Attack04: timely_generate_attack(0.5f); break;
	case Motion_Attack05: timely_generate_attack(0.45f); break;
	}

	//モーション終わりの0.2秒前以降に攻撃ボタンを押すとコンボ
	if (Input::get_button_down(PAD_INPUT_1) && has_elapsed(mesh_.anim_total_sec() - 0.2f)) {
		is_combo = true;
	}

	//コンボ条件が成立してるか？
	if (is_motion_end() && is_combo) {
		is_combo = false;
		switch (combo_counter) {
		case 0: change_state(State::Attack, Motion_Attack02, false); break;
		case 1: change_state(State::Attack, Motion_Attack03, false); break;
		case 2: change_state(State::Attack, Motion_Attack04, false); break;
		case 3: change_state(State::Attack, Motion_Attack05, false); break;
		}
		combo_counter++;
	}

	//攻撃入力が無いままモーション終了したら移動状態へ
	if (is_motion_end()) { 
		//攻撃状態をリセット
		is_combo = false;
		combo_counter = 0;
		//移動状態へ
		change_state(State::Move, Motion_Idle);
		return;
	}
}

void Player::damage(float delta_time)
{
	//TODO:ダメージ中一瞬当たり判定無効にする（連続攻撃は受けるが、同時攻撃は一つ分しか受けない）
	if (is_motion_end()) {
		change_state(State::Move, Motion_Idle);
		return;
	}
}

void Player::avoid(float delta_time)
{
	//回避最中か？
	static bool is_avoiding{ false };
	//回避開始直後なら、入力方向へローリング
	if (!is_avoiding) {
		Vector3 forward = camera_forward();
		if (Input::get_button(PAD_INPUT_DOWN)) {
			velocity_ = -forward * AvoidSpeed;
			rotation_.y = camera_angle().y + 180.0f;
		}
		else {
			velocity_ = forward * AvoidSpeed;
		}
		//回避最中へ
		is_avoiding = true;
	}

	//回避終了後移動状態へ
	if (is_motion_end()) {
		is_avoiding = false;
		change_state(State::Move, Motion_Idle);
		return;
	}
	position_ += velocity_;
}

void Player::dead(float delta_time)
{
	if (is_motion_end()) {
		die();
		return;
	}
}

void Player::generate_attack(float lifespan, float delay)
{
	//プレイヤー位置からのオフセット
	static const Vector3 AttackHeightOffset{ 0.0f, 50.0f, 0.0f };
	//攻撃判定の位置
	Vector3 position = (position_ + AttackHeightOffset) + forward() * 200.0f;
	//攻撃判定
	Sphere attack{ 80.0f, position };

	world_->add_actor(new AttackCollider{ world_, attack, "PlayerAttackTag", "AttackCollider", "PlayerTag", lifespan, delay });
}

void Player::timely_generate_attack(float time)
{
	if (has_excessed(time)) {
		sound_.play_SE(SE_SwordAttack01);
		generate_attack(0.2f);
	}
}

void Player::select_motion()
{
	//ForDebug
	if (Input::get_button_down(PAD_INPUT_1)) {
		motion_++;
		mesh_.change_anim(motion_);
	}
	if (Input::get_button_down(PAD_INPUT_2)) {
		motion_--;
		mesh_.change_anim(motion_);
	}
}

Vector3 Player::camera_angle()
{
	//カメラを取得
	Actor* camera = world_->camera();
	//無ければ終了
	if (!camera)  return 0.0f;

	//カメラの前方向を取得
	return camera->rotation();
}

Vector3 Player::camera_forward()
{
	Vector3 forward = Vector3{ 0.0f, 0.0f, 1.0f } * Matrix4x4::rotateY(camera_angle().y);
	return forward;
}

void Player::combo_attack(unsigned motion, float lifespan, float delay)
{
	change_state(State::Attack, motion, false);
	generate_attack(lifespan, delay);
}

void Player::take_damage(int damage)
{
	parameter_.hp -= damage;
	p_db_.set_hp(parameter_.hp);
}

void Player::update_state_timer_(float delta_time)
{
	prev_state_timer_ = state_timer_;
	state_timer_ += delta_time;
}

bool Player::has_elapsed(float time)
{
	return state_timer_ >= time;
}

bool Player::has_excessed(float time)
{
	return state_timer_ >= time && prev_state_timer_ < time;
}

bool Player::is_motion_end() const
{
	return state_timer_ >= mesh_.anim_total_sec();
}

