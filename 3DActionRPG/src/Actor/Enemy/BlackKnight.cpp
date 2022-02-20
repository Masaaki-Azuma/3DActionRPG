#include "BlackKnight.h"

#include <cassert>

#include "AssetsManager/Mesh.h"
#include "AssetsManager/PlayerDatabase.h"
#include "Util/DxConverter.h"
#include "Util/MyMath.h"
#include "BattleScene/IWorld.h"

const float DetectionRadius{ 400.0f }; //プレイヤーを検知する範囲半径
const float AttackRadius{ 50.0f };
const float MoveSpeed{ 200.0f };

BlackKnight::BlackKnight(IWorld* world, const Vector3& position, const Vector3& rotation)
{
	assert(DetectionRadius >= AttackRadius && "プレイヤー感知半径が不正です");

	mesh_ = SkinningMesh{ Mesh::black_knight_handle, 20.0f };
	world_ = world;
	name_ = "BlackKnight";
	tag_ = "EnemyTag";
	position_ = position;
	rotation_ = rotation;
	collider_ = Sphere{ 50.0f, Vector3{0.0f, 20.0f, 0.0f} };
	motion_ = 0;
	//parameter_ = Parameter{ 500, 100 };

	//メッシュ姿勢初期化
	mesh_.change_anim(motion_, motion_loop_, motion_interruption);
	mesh_.set_position(position_);
	mesh_.set_rotation(rotation_ * MyMath::Deg2Rad);
	mesh_.set_scale(Vector3{ 2.0f, 2.0f, 2.0f });

}

void BlackKnight::update_state(float delta_time)
{
}
