#ifndef ENEMY_H_
#define ENEMY_H_

#include "Actor/Actor.h"
#include "Actor/SkinningMesh.h"
#include "AssetsManager/EnemyDatabase.h"

class Enemy : public Actor
{
public:
	//struct Parameter
	//{
	//	int hp;             //体力
	//	int attack;         //攻撃力
	//	int max_stun_count; //被弾時に連続で硬直する最大回数
	//};

	//HACK:敵によって状態が違うためenumではなく、structでどうにかする。以下は必ず使う状態のみを記述
	struct State
	{
		static const unsigned int Move   = 0;
		static const unsigned int Attack = 1;
		static const unsigned int Damage = 2;
		static const unsigned int Dead   = 3;
		static const unsigned int DeriveTop = 4;
	};
public:
	Enemy();
	virtual ‾Enemy();

	virtual void update(float delta_time) override;
	virtual void draw() const override;
	virtual void react(Actor& other) override;

protected:
	void change_state(unsigned int state, unsigned int motion, bool loop = true, bool reset = false);
	virtual void update_state(float delta_time) = 0; 
	void generate_attack(const Sphere& collider, const std::string& name, float lifespan, float delay = 0.0f);
	void take_damage(int damage);

	//Fordebug
	void select_motion();
	virtual void draw_debug() const {};

protected:
	//HACK:アニメーション付きのアクターは別に基底クラスを設けるべき
	SkinningMesh mesh_;
	//状態
	unsigned int state_;
	//状態タイマー
	float state_timer_{ 0.0f };
	//モーション番号
	unsigned int motion_;
	//モーションループするか？
	bool motion_loop_{ true };
	//同じモーションでも最初から再生するか？
	bool motion_interruption{ false };
	//パラメーター
	EnemyDatabase::EnemyParameter parameter_;
	//敵データベース参照
	EnemyDatabase& e_DB_{ EnemyDatabase::GetInstance() };

};
#endif//!ENEMY_H_

