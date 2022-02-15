#ifndef PLAYER_H_
#define PLAYER_H_

#include "Actor.h"
#include "SkinningMesh.h"
#include "AssetsManager/PlayerDatabase.h"

class Player : public Actor
{
public:
	enum class State
	{
		Move,
		Attack,
		Damage,
		Avoid,
		Dead,
	};
	
public:
	Player(IWorld* world);
	‾Player();
	virtual void update(float delta_time) override;
	virtual void draw() const override;
	virtual void react(Actor& other) override;
	bool store_model_handle(int model_handle);
private:
	//
	void update_state(float delta_time);
	//状態を変化させる
	void change_state(State state, unsigned int motion, bool loop = true);
	//移動状態更新
	void move(float delta_time);
	//攻撃状態更新
	void attack(float delta_time);
	//ダメージ状態更新
	void damage(float delta_time);
	//回避状態更新
	void avoid(float delta_time);
	//死亡状態更新
	void dead(float delta_time);

	//攻撃判定を生成
	void generate_attack(float lifespan, float delay = 0.0f);
	//コンボ攻撃
	void combo_attack(unsigned motion, float lifespan, float delay = 0.0f);
	//ダメージを受ける
	void take_damage(int damage);

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
	//戦闘パラメーター
	PlayerDatabase::Parameter parameter_;
	//プレイヤーデータ参照
	PlayerDatabase& p_db_{ PlayerDatabase::GetInstance() };
};
#endif//!PLAYER_H_

