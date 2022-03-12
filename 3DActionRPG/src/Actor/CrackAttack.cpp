#include "CrackAttack.h"

#include "BattleScene/IWorld.h"
#include "AttackCollider.h"
#include "Util/DxConverter.h"
#include "AssetsManager/Mesh.h"

CrackAttack::CrackAttack(IWorld* world, const Vector3& position):
	mesh_{Mesh::crack_handle}
{
	world_ = world;
	position_ = position;
	position_.y = -50.0f;
	enable_collider_ = false;
	timer_.reset();
	world_->add_actor(new AttackCollider{ world, Sphere{100.0f, position},"EnemyAttackTag", "BlackKnightAttack", "BlackKnight", 0.3f });
}

void CrackAttack::update(float delta_time)
{
	//タイマー更新
	timer_.update(delta_time);
	//位置更新
	static const float MoveSpeed = 400.0f;
	position_.y = (std::min)(position_.y + MoveSpeed * delta_time, 0.0f);
	//一定時間経過で消滅
	static const float Lifespan = 2.0f;
	if (timer_.has_elapsed(Lifespan)) {
		die();
	}
}

void CrackAttack::draw() const
{
	//モデルの姿勢を変更
	DxLib::MV1SetPosition(mesh_, DxConverter::GetVECTOR(position_));
	DxLib::MV1SetRotationXYZ(mesh_, DxConverter::GetVECTOR(rotation_));

	//描画
	MV1DrawModel(mesh_);
}
