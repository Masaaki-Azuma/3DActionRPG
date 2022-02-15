#include "Mimic.h"

#include "AssetsManager/Mesh.h"
#include "AssetsManager/PlayerDatabase.h"
#include "Util/DxConverter.h"
#include "Util/MyMath.h"
#include "BattleScene/IWorld.h"

enum
{
	Motion_Attack01    = 0,
	Motion_Attack02    = 1,
	Motion_Die         = 2,
	Motion_Damage      = 4,
	Motion_Idle01      = 5,
	Motion_Immitate    = 6,
	Motion_Idle02      = 7,
	Motion_Run         = 8,
	Motion_WalkForward = 14,
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
	//parameter_ = Parameter{ 500, 100 };

	//メッシュ姿勢初期化
	mesh_.change_anim(motion_, motion_loop_, motion_interruption);
	mesh_.set_position(position_);
	mesh_.set_rotation(rotation_ * MyMath::Deg2Rad);
	mesh_.set_scale(Vector3{ 2.0f, 2.0f, 2.0f });

}

void Mimic::react(Actor& other)
{
	if (other.tag() == "PlayerAttackTag") {
		//プレイヤーの攻撃力分ダメージを受ける
		take_damage(PlayerDatabase::GetInstance().get_current_parameter().attack);
		if (parameter_.hp <= 0) {
			//当たり判定を無効化
			enable_collider_ = false;
			//敵討伐数を加算
			world_->add_basterd(name_);
			die();
			//死亡状態に遷移
			/*change_state(State::Dead, Motion_Die, false);
			mesh_.change_anim(motion_, motion_loop_, motion_interruption);*/
			return;
		}
		//ダメージ状態に
		change_state(State::Damage, Motion_Damage, false, true);
		mesh_.change_anim(motion_, motion_loop_, motion_interruption);
	}
}

void Mimic::update_state(float delta_time)
{
}
