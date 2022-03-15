#include "Player.h"

#include "Util/DxConverter.h"
#include "Util/Matrix4x4.h"
#include "Util/MyMath.h"
#include "AssetsManager/Mesh.h"
#include "AssetsManager/EnemyDatabase.h"
#include "BattleScene/IWorld.h"
#include "AttackCollider/AttackCollider.h"

const float Speed{ 10.0f };
const float AvoidSpeed{ 12.0f };
const Vector3 CollisionOffset{ 0, 50.0f, 0 };
const float CollisionRadius{ 50.0f };

//モーション番号
/*名前の衝突回避のために、無名列挙型には接頭語をつけることを推奨する*/
enum
{
	Motion_Attack01        = 0,
	Motion_Attack02        = 1,
	Motion_Attack03        = 2,
	Motion_Attack04        = 3,
	Motion_Attack05        = 4,
	Motion_Die             = 9,
	Motion_Recover         = 10,
	Motion_Damage          = 14,
	Motion_Idle            = 15,
	Motion_RollingForward  = 22,
	Motion_RollingBackward = 23,
	Motion_RunForward      = 26,
	Motion_RunBackward     = 27,
	Motion_Sprint          = 29,
};

Player::Player(IWorld* world, const Vector3& position):
	mesh_{Mesh::player_handle, 30.0f},
	state_{State::Move},
	state_timer_{0.0f},
	motion_{Motion_Idle},
	motion_loop_{true}
{
	world_ = world;
	name_ = "Player";
	tag_ = "PlayerTag";
	position_ = position;
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
	//メッシュを描画
	mesh_.draw();
}

void Player::react(Actor& other)
{
	if (other.tag() == "EnemyAttackTag") {
		sound_.play_SE(SE_Damage);
		//敵の攻撃に割り込まれたら、コンボリセット
		combo_counter_ = 0;
		//被弾中は無敵状態
		enable_collider_ = false;
		int damage = EnemyDatabase::GetInstance().get_attack(other.name());
		take_damage(damage);
		if (parameter_.hp <= 0) {
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

	//移動状態のモーション番号
	unsigned int motion = Motion_Idle;
	//カメラの前方向を取得
	Vector3 cam_forward = camera_forward();
	//移動
	Vector2 dir_Lstick = input_.GetLeftStick();
	if (dir_Lstick == Vector2::ZERO) {
		velocity_ = Vector3::ZERO;
	}
	else {
		//カメラ右＋正面入力で-90度？
		float angle = Vector3::SignedAngleY(Vector3::FORWARD, Vector3{ dir_Lstick.x, 0.0f, dir_Lstick.y });
		Vector3 velocity = cam_forward.Normalize() * Matrix4x4::rotateY(angle * MyMath::Rad2Deg) * Speed;
		velocity_ = velocity;
		rotation_.y = Vector3::SignedAngleY(Vector3::FORWARD, velocity_) * MyMath::Rad2Deg ;
		motion = Motion_Sprint;
	}

	//回避
	if (input_.GetButtonDown(XINPUT_BUTTON_A)) {
		change_state(State::Avoid, Motion_RollingForward, false);
		sound_.play_SE(SE_Avoid);
		return;
	}

	//攻撃
	if (input_.GetButtonDown(XINPUT_BUTTON_B)) {
		change_state(State::Attack, Motion_Attack01, false);
		return;
	}

	if (velocity_ != Vector3::ZERO && !sound_.check_SE_playing(SE_GroundWalk)) {
		sound_.play_SE(SE_GroundWalk);
	}
	position_ += velocity_;
	change_state(State::Move, motion);
}

void Player::attack(float delta_time)
{
	//各攻撃モーションの適切なタイミングで攻撃判定を生成
	switch (motion_) {
	case Motion_Attack01: timely_generate_attack(0.2f); break;
	case Motion_Attack02: timely_generate_attack(0.2f); break;
	case Motion_Attack03: timely_generate_attack(0.15f); break;
	case Motion_Attack04: timely_generate_attack(0.5f); break;
	case Motion_Attack05: timely_generate_attack(0.45f); break;
	}

	//モーション終わりに攻撃ボタンを押しているとコンボ
	if (input_.GetButton(XINPUT_BUTTON_B) && is_motion_end()) {
		switch (combo_counter_) {
		case 0: change_state(State::Attack, Motion_Attack02, false); break;
		case 1: change_state(State::Attack, Motion_Attack03, false); break;
		case 2: change_state(State::Attack, Motion_Attack04, false); break;
		case 3: change_state(State::Attack, Motion_Attack05, false); break;
		}
		combo_counter_++;
	}

	//攻撃入力が無いままモーション終了したら移動状態へ
	if (is_motion_end()) { 
		//攻撃状態をリセット
		combo_counter_ = 0;
		//移動状態へ
		change_state(State::Move, Motion_Idle);
		return;
	}
}

void Player::damage(float delta_time)
{
	if (is_motion_end()) {
		enable_collider_ = true;
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
		//回避最中へ
		is_avoiding = true;
		velocity_ = forward() * AvoidSpeed;
	}

	position_ += velocity_;

	//回避終了後移動状態へ
	if (is_motion_end()) {
		is_avoiding = false;
		change_state(State::Move, Motion_Idle);
		return;
	}
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

	world_->add_actor(std::make_shared<AttackCollider>(world_, attack, "PlayerAttackTag", "AttackCollider", "PlayerTag", lifespan, delay));
}

void Player::timely_generate_attack(float time)
{
	if (has_excessed(time)) {
		sound_.play_SE(SE_SwordAttack01);
		generate_attack(0.2f);
	}
}

Vector3 Player::camera_angle()
{
	//カメラを取得
	std::shared_ptr<Actor> camera = world_->camera();
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
	parameter_.hp = (std::max)(parameter_.hp - damage, 0);
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

