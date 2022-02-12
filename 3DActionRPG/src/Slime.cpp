#include "Slime.h"
#include <cassert>
#include "Mesh.h"
#include "IWorld.h"
#include "DxConverter.h"
#include "Util/MyMath.h"

enum //モーション
{
	Motion_Attack01 = 0,
	Motion_Attack02 = 1,
	Motion_Die = 2,
	Motion_Damage = 4,
	Motion_Idle = 5,
	Motion_Run = 7,
	Motion_WalkForward = 12,
};

const float DetectionRadius{ 400.0f }; //プレイヤーを検知する範囲半径
const float AttackRadius{ 50.0f };
const float MoveSpeed{ 200.0f };

Slime::Slime(IWorld* world, const Vector3& position, const Vector3& rotation)
{
	assert(DetectionRadius >= AttackRadius && "プレイヤー感知半径が不正です");

	//ForDebug
	mesh_ = SkinningMesh{ Mesh::slime_handle, 30.0f };
	world_ = world;
	name_ = "Slime";
	tag_ = "EnemyTag";
	position_ = position;
	rotation_ = rotation;
	collider_ = Sphere{ 50.0f, Vector3{0.0f, 20.0f, 0.0f} };
	motion_ = 0;
	parameter_ = Parameter{ 500, 100 };
}

void Slime::late_update(float delta_time)
{
}

void Slime::react(Actor& other)
{
	if (other.tag() == "PlayerAttackTag") {
		change_state(State::Damage, Motion_Damage, false, true);
		mesh_.change_anim(motion_, motion_loop_, motion_reset_);
	}
}

void Slime::update_state(float delta_time)
{
	motion_reset_ = false;
	switch (state_) {
	case State::Move:   move(delta_time);   break;
	case State::Attack: attack(delta_time); break;
	case State::Damage: damage(delta_time); break;
	case State::Dead:   dead(delta_time);   break;
	}

	state_timer_ += delta_time;
}

void Slime::move(float delta_time)
{
	//プレイヤーが存在しなかったら棒立ち状態
	Actor* player = world_->find_actor("Player");
	if (!player) {
		change_state(State::Move, Motion_Idle);
		return;
	}
	//
	unsigned int motion = Motion_Idle;
	Vector3 velocity = Vector3::ZERO;

	//プレイヤー方向
	Vector3 direction = player->position() - position_;
	direction.y = 0.0f;
	//プレイヤーとの距離
	float distance = direction.Length();
	//距離により状態
	if (distance <= AttackRadius) { //攻撃状態
		Vector3 pos_attack = position_ + forward() * 100.0f;
		generate_attack(Sphere{50.0f, pos_attack}, 0.5f, 0.4f);
		change_state(State::Attack, Motion_Attack02, false);
		return;
	}
	else if (distance <= DetectionRadius) {  //移動状態
		velocity = direction.Normalize() * MoveSpeed;

		//向いている方向から回転角度を求める
		rotation_.y = Vector3::SignedAngleY(Vector3::FORWARD, direction)  * MyMath::Rad2Deg;

		motion = Motion_WalkForward;
	}

	velocity_ = velocity;
	position_ += velocity_ * delta_time;
	change_state(State::Move, motion);
}

void Slime::attack(float delta_time)
{
	if (state_timer_ >= mesh_.anim_total_sec()) {
		change_state(State::Move, Motion_Idle);
	}
}

void Slime::damage(float delta_time)
{
	if (state_timer_ >= mesh_.anim_total_sec()) {
		change_state(State::Move, Motion_Idle);
	}
}

void Slime::dead(float delta_time)
{
}



void Slime::draw_debug() const
{
	static const int green = DxLib::GetColor(0, 255, 0);
	static const int red = DxLib::GetColor(255, 0, 0);
	DrawSphere3D(DxConverter::GetVECTOR(position_), DetectionRadius, 4, green, green, false);
	DrawSphere3D(DxConverter::GetVECTOR(position_), AttackRadius, 4, red, red, false);
}
