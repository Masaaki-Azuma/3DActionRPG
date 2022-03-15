#include "JewelAttack.h"

#include <vector>

#include "AssetsManager/Mesh.h"
#include "Util/MyRandom.h"
#include "Util/DxConverter.h"

static const std::vector<DxLib::COLOR_F> color_list_{
	DxLib::GetColorF(0.2f, 1.0f, 1.0f, 1.0f), //水色
	DxLib::GetColorF(1.0f, 0.2f, 1.0f, 1.0f), //紫
	DxLib::GetColorF(1.0f, 1.0f, 0.2f, 1.0f), //金
	DxLib::GetColorF(1.0f, 0.2f, 0.2f, 1.0f), //赤
	DxLib::GetColorF(0.2f, 1.0f, 0.2f, 1.0f)  //緑
};
static const float Gravity{ -600.0f };

JewelAttack::JewelAttack(IWorld* world, const Vector3& position, const Vector3& velocity) :
	AttackCollider{ world, Sphere{15.0f}, "EnemyAttackTag","MimicAttack", "EnemyTag", 2.0f},
	mesh_{Mesh::jewel_handle}
{
	//位置設定
	position_ = position;
	//速度設定
	velocity_ = velocity;
	//宝石の色をランダムに決定
	int rand_index = MyRandom::Range(0, color_list_.size() - 1);
	color_ = color_list_[rand_index];
	//回転速度をランダムに設定
	const float MaxRotSpeed = 10.0f;
	rot_velocity_ = Vector3{
		MyRandom::Range(-MaxRotSpeed, MaxRotSpeed),
		MyRandom::Range(-MaxRotSpeed, MaxRotSpeed),
		MyRandom::Range(-MaxRotSpeed, MaxRotSpeed) };

}

void JewelAttack::update(float delta_time)
{
	AttackCollider::update(delta_time);
	move(delta_time);
}

void JewelAttack::draw() const
{
	//姿勢の変更
	DxLib::MV1SetPosition(mesh_, DxConverter::GetVECTOR(position_));
	DxLib::MV1SetRotationXYZ(mesh_, DxConverter::GetVECTOR(rotation_));
	//色の変更
	DxLib::MV1SetMaterialDifColor(mesh_, 0, color_);
	//モデルの描画
	DxLib::MV1DrawModel(mesh_);

}

void JewelAttack::react(Actor& other)
{
	if (other.name() == "Player") {
		die();
	}
}

void JewelAttack::move(float delta_time)
{
	velocity_.y += Gravity * delta_time;
	position_ += velocity_ * delta_time;
	rotation_ += rot_velocity_ * delta_time;
}
