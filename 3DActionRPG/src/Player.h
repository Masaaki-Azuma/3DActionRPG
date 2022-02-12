#ifndef PLAYER_H_
#define PLAYER_H_

#include "Actor.h"
#include "SkinningMesh.h"

class Player : public Actor
{
public:
	enum class State
	{
		Move,
		Attack,
		Damage,
		Avoid,
	};
public:
	Player(IWorld* world);
	virtual void update(float delta_time) override;
	virtual void draw() const override;
	virtual void react(Actor& other) override;
	bool store_model_handle(int model_handle);
private:
	//
	void update_state(float delta_time);
	//状態を変化させる
	void change_state(State state, unsigned int motion, bool loop = true);
	void move(float delta_time);
	void attack(float delta_time);
	void damage(float delta_time);
	void avoid(float delta_time);
	void generate_attack(float lifespan, float delay = 0.0f);
	void combo_attack(unsigned motion, float lifespan, float delay = 0.0f);

	//ForDebug
	void select_motion();
	Vector3 camera_angle();
	Vector3 camera_forward();

private:
	//アニメーションメッシュ
	SkinningMesh  mesh_;
	//状態
	State state_;
	//状態タイマー
	float state_timer_;
	//モーション番号
	unsigned int motion_;
	//モーションループするか？
	bool motion_loop_;
};
#endif//!PLAYER_H_

