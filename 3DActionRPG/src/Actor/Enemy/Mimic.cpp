#include "Mimic.h"

#include "AssetsManager/Mesh.h"
#include "AssetsManager/PlayerDatabase.h"
#include "Util/DxConverter.h"
#include "Util/MyMath.h"
#include "Util/Matrix4x4.h"
#include "BattleScene/IWorld.h"
#include "Actor/AttackCollider/JewelAttack.h"

enum
{
	Motion_Attack01     = 0,
	Motion_Attack02     = 1,
	Motion_Die          = 2,
	Motion_Damage       = 4,
	Motion_Idle01       = 5,
	Motion_Imitate      = 6,
	//Motion_Idle02       = 7,
	Motion_Run          = 8,
	Motion_Surprise     = 11,
	Motion_Rage         = 12,
	Motion_WalkBackward = 13,
	Motion_WalkForward  = 14,
};

static const float ActiveRadius{ 450.0f };     //imitate状態を解除する範囲半径
static const float DetectionRadius{ 1200.0f }; //プレイヤーを検知する範囲半径
static const float EscapeRadius{ 150.0f };            //
static const float AttackRadius{ 200.0f };     //プレイヤーを攻撃し始める範囲半径
static const float MoveSpeed{ 200.0f };        //移動スピード
static const float DashSpeed{ 400.0f };

Mimic::Mimic(IWorld* world, const Vector3& position, const Vector3& rotation):
	Enemy{world, position, rotation}
{
	name_ = "Mimic";
	move_speed_ = MoveSpeed;
	collider_ = Sphere{ 100.0f, Vector3{0.0f, 100.0f, 0.0f} };
	state_ = StateMimic::Imitate;
	motion_ = Motion_Imitate;
	parameter_ = e_DB_.get_parameter(name_);

	//メッシュ姿勢初期化
	mesh_ = SkinningMesh{ Mesh::GetInstance().mesh_handle(Mesh_Mimic), 20.0f };
	mesh_.change_anim(motion_, motion_loop_, motion_interruption);
	mesh_.set_position(position_);
	mesh_.set_rotation(rotation_ * MyMath::Deg2Rad);
	mesh_.set_scale(Vector3{ 2.0f, 2.0f, 2.0f });

}

void Mimic::react_player_attack(Actor& other)
{
	if (other.tag() == "PlayerAttackTag") {
		//プレイヤーの攻撃力分ダメージを受ける
		take_damage(PlayerDatabase::GetInstance().get_current_parameter().attack);
		if (state_ == StateMimic::Rage)  return;
		//ひるみ回数を増やす
		flinch_count_++;
		if (parameter_.hp <= 0) {
			//当たり判定を無効化
			enable_collider_ = false;
			//死亡状態に遷移
			change_state(State::Dead, Motion_Die, false);
			mesh_.change_anim(motion_, motion_loop_, motion_interruption);
			return;
		}
		//ダメージ状態に
		change_state(State::Damage, Motion_Damage, false, true);
		mesh_.change_anim(motion_, motion_loop_, motion_interruption);

	}
}

void Mimic::update_state(float delta_time)
{
	switch (state_) {
	case StateMimic::Move:       move(delta_time);        break;
	case StateMimic::Attack:     attack(delta_time);      break;
	case StateMimic::Damage:     damage(delta_time);      break;
	case StateMimic::Dead:       dead(delta_time);        break;
	case StateMimic::Imitate:    imitate(delta_time);     break;
	case StateMimic::LongAttack: long_attack(delta_time); break;
	case StateMimic::Surprise:   surprise(delta_time);    break;
	case StateMimic::Rage:       rage(delta_time);        break;
	}
}

void Mimic::move(float delta_time)
{
	//プレイヤーが存在しなかったら棒立ち状態
	std::shared_ptr<Actor> player = find_player();
	if (!player) {
		change_motion(Motion_Idle01);
		return;
	}

	//移動モーション
	unsigned int motion = Motion_Idle01;
	//移動量
	Vector3 velocity = Vector3::ZERO;
	//プレイヤーとの距離
	float distance = get_vec_to_player().Length();
	//距離により状態
	if (distance <= EscapeRadius) {
		velocity = make_distance();
		//後ろ歩きモーションを設定
		motion = Motion_WalkBackward;
	}
	else if (distance <= AttackRadius) {
		//攻撃状態に遷移
		change_state(State::Attack, Motion_Attack01, false);
		return;
	}
	else if (distance <= DetectionRadius) {  //移動状態
		velocity = make_approach();
		//前歩きモーションを設定
		motion = Motion_WalkForward;
		//一定時間移動状態が続いたら長射程攻撃
		if (has_elapsed(5.0f)) {
			sound_.play_SE(SE_MimicLongAttack);
			change_state(StateMimic::LongAttack, Motion_Attack02, false);
			return;
		}
	}
	else {
		if (has_elapsed(3.0f)) {
			sound_.play_SE(SE_MimicRage, PlayMode::Loop);
			//HACK:状態変化直後のみ呼ばれる処理を追加するべきではないか
			Vector3 toward_player = (player->position() - position_).Normalize();
			toward_player.y = 0.0f;
			velocity_ = toward_player * DashSpeed;
			rotation_.y = degree_forward(velocity_);
			change_state(StateMimic::Rage, Motion_Rage);
			return;
		}
	}

	velocity_ = velocity;
	position_ += velocity_ * delta_time;
	change_motion(motion);
}

void Mimic::attack(float delta_time)
{
	if (can_generate_attack(0.4f)) {
		Vector3 pos_attack = position_ + forward() * 200.0f;
		generate_attack(Sphere{ 100.0f, pos_attack }, name_ + "Attack", 0.5f);
	}
	if (is_motion_end()) {
		change_state(State::Move, Motion_Idle01);
	}
}

void Mimic::damage(float delta_time)
{
	if (is_motion_end()) {
		//プレイヤーへ向き直る
		make_approach();
		change_state(State::Attack, Motion_Attack01, false);
	}
}


void Mimic::imitate(float delta_time)
{
	std::shared_ptr<Actor> player = find_player();
	if (!player) return;
	
	//プレイヤーとの距離
	float distance = get_vec_to_player().Length();
	if (distance > ActiveRadius) return;
	change_state(StateMimic::Surprise, Motion_Surprise, false);
}

void Mimic::long_attack(float delta_time)
{
	if (can_generate_attack(0.4f)) {
		Vector3 pos_attack = position_ + forward() * 150.0f;
		generate_attack(Sphere{ 150.0f, pos_attack }, name_ + "Attack", 0.3f);
		pos_attack = position_ + forward() * 400.0f;
		generate_attack(Sphere{ 150.0f, pos_attack }, name_ + "Attack", 0.3f);
	}
	if (is_motion_end()) {
		change_state(StateMimic::Move, Motion_Idle01);
	}
}

void Mimic::surprise(float delta_time)
{
	if (is_motion_end()) {
		change_state(StateMimic::Move, Motion_Idle01);
	}
}

void Mimic::rage(float delta_time)
{
	position_ += velocity_ * delta_time;
	//一定時間ごとにばらまき攻撃
	for (int i = 1; i < 5; ++i) {
		//ばらまき間隔
		const float ScatterInterval = 1.0f;
		if (has_excessed(ScatterInterval * i)) {
			//宝石をばらまく
			scatter_jewel();
		}
	}

	if (has_elapsed(5.0f)) {
		sound_.stop_SE(SE_MimicRage);
		change_state(StateMimic::Move, Motion_Idle01);
	}
}

void Mimic::scatter_jewel()
{
	//ばらまく宝石の数
	static const int NumJewel = 8;
	static const float JewelHeight = 300.0f;
	for (int i = 0; i < NumJewel; ++i) {
		Vector3 jewel_velocity = Vector3{ 0.0f, 450.0f, 80.0f };
		jewel_velocity = jewel_velocity * Matrix4x4::rotateY(360.0f / NumJewel * i);
		Vector3 jewel_position = position_;
		jewel_position.y = JewelHeight;
		world_->add_actor(std::make_shared<JewelAttack>( world_, jewel_position, jewel_velocity ));
	}
}

void Mimic::draw_debug() const
{
	static const int yellow = DxLib::GetColor(255, 255, 0);
	static const int red = DxLib::GetColor(255, 0, 0);
	DrawSphere3D(DxConverter::GetVECTOR(position_), DetectionRadius, 4, yellow, yellow, false);
	DrawSphere3D(DxConverter::GetVECTOR(position_), AttackRadius, 4, red, red, false);
	DxLib::DrawFormatString(0, 20, DxLib::GetColor(255, 255, 255), "mimic_hp:%d", parameter_.hp);
}
