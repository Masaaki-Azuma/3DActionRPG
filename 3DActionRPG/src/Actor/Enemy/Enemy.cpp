#include "Enemy.h"

#include "Util/MyMath.h"
#include "Util/Input.h"
#include "AssetsManager/Mesh.h"
#include "BattleScene/IWorld.h"
#include "Actor/AttackCollider.h"

Enemy::Enemy()
{
}

Enemy::‾Enemy()
{
	//HACK:モデルハンドルの複製場所はSkinningMeshコンストラクタだがわかりづらくないか？
	mesh_.clear();
}

void Enemy::update(float delta_time)
{
	//
	motion_interruption = false;

	//現在の状態を更新
	update_state(delta_time);
	//壁との押し出し処理
	react_wall();
	//ForDebug
	//select_motion();

	//メッシュの更新
	mesh_.change_anim(motion_, motion_loop_, motion_interruption);
	mesh_.set_position(position_);
	mesh_.set_rotation(rotation_ * MyMath::Deg2Rad);
	mesh_.update(delta_time);
}

void Enemy::draw() const
{
	mesh_.draw();

	//Fordebug
	collider().draw();
	draw_debug();
}

void Enemy::react(Actor& other){}

void Enemy::change_state(unsigned int state, unsigned int motion, bool loop, bool is_interruptive)
{
	state_ = state;
	motion_ = motion;
	motion_loop_ = loop;
	motion_interruption = is_interruptive;
	state_timer_ = 0.0f;
}

void Enemy::change_motion(unsigned int motion, bool loop)
{
	motion_ = motion;
	motion_loop_ = loop;
}

void Enemy::generate_attack(const Sphere& collider, const std::string& name, float lifespan, float delay)
{
	world_->add_actor(new AttackCollider{ world_, collider, "EnemyAttackTag", name, "EnemyTag", lifespan, delay });
}

void Enemy::take_damage(int damage)
{
	parameter_.hp -= damage;
}

void Enemy::select_motion()
{
	//ForDebug
	if (Input::get_button_down(PAD_INPUT_5)) { //S
		motion_++;
		mesh_.change_anim(motion_);
	}
	if (Input::get_button_down(PAD_INPUT_4)) { //A
		motion_--;
		mesh_.change_anim(motion_);
	}
}