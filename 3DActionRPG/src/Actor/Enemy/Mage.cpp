#include "Mage.h"

#include <cassert>

#include "Util/DxConverter.h"
#include "Util/MyMath.h"
#include "AssetsManager/Mesh.h"
#include "AssetsManager/PlayerDatabase.h"
#include "BattleScene/IWorld.h"

enum
{
	Motion_Attack01 = 0,
	Motion_Attack02 = 1,
	Motion_Die      = 2,
	Motion_Dizzy    = 3,
	Motion_Damage   = 4,
	Motion_Idle     = 5,
	Motion_WalkBackward = 12,
};

static const float DetectionRadius{ 800.0f }; //プレイヤーを検知する範囲半径
static const float EscapeRadius{ 200.0f };
static const float MoveSpeed{ 200.0f };

Mage::Mage(IWorld* world, const Vector3& position, const Vector3& rotation):
	Enemy{ world, position, rotation }
{

	name_ = "Mage";
	collider_ = Sphere{ 100.0f, Vector3{0.0f, 20.0f, 0.0f} };
	state_ = State::Move;
	motion_ = Motion_Idle;
	parameter_ = e_DB_.get_parameter(name_);

	//メッシュ姿勢初期化
	mesh_ = SkinningMesh{ Mesh::mage_handle, 30.0f };
	mesh_.change_anim(motion_, motion_loop_, motion_interruption);
	mesh_.set_position(position_);
	mesh_.set_rotation(rotation_ * MyMath::Deg2Rad);
}

void Mage::react(Actor& other)
{

}

void Mage::update_state(float delta_time)
{
	switch (state_) {
	case  State::Move: move(delta_time); break;
	}
}

void Mage::move(float delta_time)
{
	//プレイヤーが存在しなかったら棒立ち状態
	std::shared_ptr<Actor> player = world_->find_actor("Player");
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
	if (distance <= EscapeRadius) {
		velocity = direction.Normalize() * -MoveSpeed;
		//向いている方向から回転角度を求める
		rotation_.y = Vector3::SignedAngleY(Vector3::FORWARD, direction) * MyMath::Rad2Deg;

		motion = Motion_WalkBackward;

	}
	else if (distance <= DetectionRadius) {  //移動状態

		/*Vector3 pos_attack = position_ + forward() * 100.0f;
		generate_attack(Sphere{ 50.0f, pos_attack }, "MageAttack", 0.5f, 0.4f);
		change_state(State::Attack, Motion_Attack02, false);
		return;*/
		//velocity = direction.Normalize() * MoveSpeed;

		//向いている方向から回転角度を求める
		rotation_.y = Vector3::SignedAngleY(Vector3::FORWARD, direction) * MyMath::Rad2Deg;

	}

	velocity_ = velocity;
	position_ += velocity_ * delta_time;
	change_state(State::Move, motion);
}

void Mage::draw_debug() const
{
	static const int yellow = DxLib::GetColor(0, 255, 0);
	static const int red = DxLib::GetColor(255, 0, 0);
	static const int blue = DxLib::GetColor(0, 0, 255);
	DrawSphere3D(DxConverter::GetVECTOR(position_), DetectionRadius, 4, yellow, yellow, false);
	DrawSphere3D(DxConverter::GetVECTOR(position_), EscapeRadius, 4, blue, blue, false);
	DxLib::DrawFormatString(0, 20, DxLib::GetColor(255, 255, 255), "mage_hp:%d", parameter_.hp);
}
