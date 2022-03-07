#ifndef SLIME_H_
#define SLIME_H_

#include "Enemy.h"

class Slime : public Enemy
{
	struct StateSlime : Enemy::State
	{
		static const int Idle = State::DeriveTop;
		static const int Dash = State::DeriveTop + 1;
	};
public:
	Slime(IWorld* world, const Vector3& position, const Vector3& rotation = Vector3{0.0f, 180.0f, 0.0f});
	virtual void react_player_attack(Actor& other) override;
protected:
	virtual void update_state(float delta_time) override;
private:
	void idle(float delta_time);
	void move(float delta_time);
	void attack(float delta_time);
	void dash(float delta_time);
	void damage(float delta_time);

	//ForDebug
	virtual void draw_debug() const override;
};
#endif//!SLIME_H_

