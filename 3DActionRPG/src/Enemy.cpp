#include "Enemy.h"
#include "Mesh.h"
#include "Util/MyMath.h"
#include "Input.h"
#include "AttackCollider.h"
#include "IWorld.h"

Enemy::Enemy()
{
}

void Enemy::update(float delta_time)
{
	//現在の状態を更新
	update_state(delta_time);
	//壁との押し出し処理
	react_wall();
	//ForDebug
	//select_motion();

	//メッシュの更新
	mesh_.change_anim(motion_, motion_loop_, motion_reset_);
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

void Enemy::change_state(unsigned int state, unsigned int motion, bool loop, bool reset)
{
	state_ = state;
	motion_ = motion;
	motion_loop_ = loop;
	motion_reset_ = reset;
	state_timer_ = 0.0f;
}

void Enemy::generate_attack(const Sphere& collider, float lifespan, float delay)
{
	world_->add_actor(new AttackCollider{ world_, collider, "EnemyAttackTag", "AttackCollider", "EnemyTag", lifespan, delay });
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