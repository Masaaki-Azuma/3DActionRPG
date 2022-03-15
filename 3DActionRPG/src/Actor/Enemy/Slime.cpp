#include "Slime.h"

#include <cassert>

#include "Util/DxConverter.h"
#include "Util/MyMath.h"
#include "AssetsManager/Mesh.h"
#include "AssetsManager/PlayerDatabase.h"
#include "BattleScene/IWorld.h"

enum //モーション
{
	Motion_Attack01    = 0,
	Motion_Attack02    = 1,
	Motion_Die         = 2,
	Motion_Damage      = 4,
	Motion_Idle        = 5,
	Motion_Run         = 7,
	Motion_WalkForward = 12,
};

static const float DetectionRadius{ 800.0f }; //プレイヤーを検知する範囲半径
static const float AttackRadius{ 150.0f };
static const float MoveSpeed{ 200.0f };
static const float DashSpeed{ 350.0f };

Slime::Slime(IWorld* world, const Vector3& position, const Vector3& rotation):
	Enemy{ world, position, rotation }
{
	assert(DetectionRadius >= AttackRadius && "プレイヤー感知半径が不正です");

	name_ = "Slime";
	move_speed_ = MoveSpeed;
	collider_ = Sphere{ 50.0f, Vector3{0.0f, 20.0f, 0.0f} };
	motion_ = Motion_Idle;
	parameter_ = e_DB_.get_parameter(name_);

	//メッシュ姿勢初期化
	mesh_ = SkinningMesh{ Mesh::slime_handle, 30.0f };
	mesh_.change_anim(motion_, motion_loop_, motion_interruption);
	mesh_.set_position(position_);
	mesh_.set_rotation(rotation_ * MyMath::Deg2Rad);
}

void Slime::react_player_attack(Actor& other)
{
	if (other.tag() == "PlayerAttackTag") {
		//プレイヤーの攻撃力分ダメージを受ける
		take_damage(PlayerDatabase::GetInstance().get_current_parameter().attack);
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

void Slime::update_state(float delta_time)
{
	switch (state_) {
	case State::Move:   move(delta_time);   break;
	case State::Attack: attack(delta_time); break;
	case State::Damage: damage(delta_time); break;
	case State::Dead:   dead(delta_time);   break;
	case StateSlime::Idle: idle(delta_time); break;
	case StateSlime::Dash: dash(delta_time); break;
	}
}

void Slime::idle(float delta_time)
{
	if (has_elapsed(1.2f)) {
		change_state(StateSlime::Move, Motion_Idle);
	}
}

void Slime::move(float delta_time)
{
	//プレイヤーが存在しなかったら棒立ち状態
	std::shared_ptr<Actor> player = find_player();
	if (!player) {
		change_state(State::Move, Motion_Idle);
		return;
	}
	//
	unsigned int motion = Motion_Idle;
	Vector3 velocity = Vector3::ZERO;

	//プレイヤーとの距離
	float distance = get_vec_to_player().Length();
	//距離により状態
	if (distance <= AttackRadius) { //攻撃状態
		//プレイヤー方向に向き直る
		make_approach();
		change_state(State::Attack, Motion_Attack02, false);
		return;
	}
	else if (distance <= DetectionRadius) {  //移動状態
		velocity = make_approach();
		motion = Motion_WalkForward;
		if (has_elapsed(3.0f)) {
			velocity_ = velocity.Normalize() * DashSpeed;
			change_state(StateSlime::Dash, Motion_Run);
			return;
		}
	}

	velocity_ = velocity;
	position_ += velocity_ * delta_time;
	change_motion(motion);
}

void Slime::attack(float delta_time)
{
	if (can_generate_attack(0.4f)) {
		sound_.play_SE(SE_SlimeAttack);
		Vector3 pos_attack = position_ + forward() * 100.0f;
		generate_attack(Sphere{ 50.0f, pos_attack }, "SlimeAttack", 0.2f);

	}
	if (is_motion_end()) {
		change_state(StateSlime::Idle, Motion_Idle);
	}
}

void Slime::dash(float delta_time)
{
	//一定方向へダッシュ
	position_ += velocity_ * delta_time;

	//ダッシュ軌道上に攻撃判定を生成
	static const int NumAttack = 4;
	static const float AttackInterval = 0.7f;
	for (int i = 0; i < NumAttack; ++i) {
		if (has_excessed(i * AttackInterval - 0.15f)) {
			sound_.play_SE(SE_SlimeBounce);
			Vector3 pos_attack = position_ + forward() * 100.0f;
			generate_attack(Sphere{ 50.0f, pos_attack }, "SlimeAttack", 0.3f);
		}
	}

	if (has_elapsed(2.65f)) {
		change_state(StateSlime::Idle, Motion_Idle);
	}
}

void Slime::damage(float delta_time)
{
	if (is_motion_end()) {
		change_state(State::Move, Motion_Idle);
	}
}

void Slime::draw_debug() const
{
	static const int green = DxLib::GetColor(0, 255, 0);
	static const int red = DxLib::GetColor(255, 0, 0);
	DrawSphere3D(DxConverter::GetVECTOR(position_), DetectionRadius, 4, green, green, false);
	DrawSphere3D(DxConverter::GetVECTOR(position_), AttackRadius, 4, red, red, false);
	DxLib::DrawFormatString(0, 20, DxLib::GetColor(255, 255, 255), "slime_hp:%d", parameter_.hp);

}
