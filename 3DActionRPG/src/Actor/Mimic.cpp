#include "Mimic.h"

#include "AssetsManager/Mesh.h"
#include "AssetsManager/PlayerDatabase.h"
#include "Util/DxConverter.h"
#include "Util/MyMath.h"
#include "BattleScene/IWorld.h"

enum
{
	Attack01    = 0,
	Attack02    = 1,
	Die         = 2,
	Damage      = 4,
	Idle01      = 5,
	Immitate    = 6,
	Idle02      = 7,
	Run         = 8,
	WalkForward = 14,
};

Mimic::Mimic(IWorld* world, const Vector3& position, const Vector3& rotation)
{
	mesh_ = SkinningMesh{ Mesh::mimic_handle, 20.0f };
	world_ = world;
	name_ = "Mimic";
	tag_ = "EnemyTag";
	position_ = position;
	rotation_ = rotation;
	collider_ = Sphere{ 50.0f, Vector3{0.0f, 20.0f, 0.0f} };
	motion_ = 0;
	parameter_ = Parameter{ 500, 100 };

	//メッシュ姿勢初期化
	mesh_.change_anim(motion_, motion_loop_, motion_interruption);
	mesh_.set_position(position_);
	mesh_.set_rotation(rotation_ * MyMath::Deg2Rad);
	mesh_.set_scale(Vector3{ 2.0f, 2.0f, 2.0f });

}

void Mimic::update_state(float delta_time)
{
}
