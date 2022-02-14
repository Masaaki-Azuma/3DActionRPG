#include "AttackCollider.h"

AttackCollider::AttackCollider(IWorld* world, const Sphere& collider,
	const std::string& tag, const std::string& name,
	const std::string& owner_tag, float lifespan, float delay):
	lifespan_timer_{lifespan}, delay_timer_{delay}
{
	world_ = world;
	name_ = name;
	tag_ = tag;
	//生成者のタグ名
	owner_tag_ = owner_tag;
	//衝突判定を初期化
	collider_ = Sphere{ collider.radius };
	//座標の初期化
	position_ = collider.center;
	//衝突判定を無効にする
	enable_collider_ = false;
}

void AttackCollider::update(float delta_time)
{
	//遅延時間が経過したら衝突判定を有効にする
	if (delay_timer_ <= 0.0f) {
		//衝突判定を有効にする
		enable_collider_ = true;
		//寿命が尽きたら死亡
		if (lifespan_timer_ <= 0.0f) {
			die();
		}
		//寿命の更新
		lifespan_timer_ -= delta_time;
	}
	//遅延時間の更新
	delay_timer_ -= delta_time;
}

void AttackCollider::draw() const
{
	//衝突判定のデバッグ表示
	if (enable_collider_) collider().draw();
}

void AttackCollider::react(Actor& other)
{
	//自分と同じタグorオーナーと同じタグは衝突判定しない
	if (other.tag() == tag() || other.tag() == owner_tag_) return;

		//衝突したら死亡
		die();
}
