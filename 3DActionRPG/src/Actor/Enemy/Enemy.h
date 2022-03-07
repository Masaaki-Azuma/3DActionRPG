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
	Enemy(IWorld* world, const Vector3& position, const Vector3& rotation);
	virtual ‾Enemy();

	virtual void update(float delta_time) override;
	virtual void draw() const override;
	virtual void react(Actor& other) override;

protected:
	virtual void react_player_attack(Actor& other);
	void change_state(unsigned int state, unsigned int motion, bool loop = true, bool reset = false);
	void change_motion(unsigned int motion, bool loop = true);
	virtual void update_state(float delta_time) = 0;
	void update_state_timer(float delta_time);
	//状態関数
	virtual void dead(float delta_time);

	//プレイヤーを検索
	Actor* find_player();
	//プレイヤー方向のベクトルを取得
	Vector3 get_vec_to_player();
	//距離を取る
	Vector3 make_distance();
	//近づく
	Vector3 make_approach();
	//攻撃判定を生成
	void generate_attack(const Sphere& collider, const std::string& name, float lifespan, float delay = 0.0f);
	//ダメージを受ける
	void take_damage(int damage);
	//攻撃判定を生成できるタイミングか？
	bool can_generate_attack(float time) const;
	//ある状態になってから指定時間を経過しているか？
	bool has_elapsed(float time);
	//ある状態になってからちょうど指定時間をすぎたタイミングか？
	bool has_excessed(float time);
	//モーションが終了したか？
	bool is_motion_end() const;

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
	//状態タイマー履歴
	float prev_state_timer_{ 0.0f };
	//モーション番号
	unsigned int motion_;
	//モーションループするか？
	bool motion_loop_{ true };
	//同じモーションでも最初から再生するか？
	bool motion_interruption{ false };
	//移動速さ
	float move_speed_{ 0.0f };
	//1攻撃判定を生成したか？
	bool has_attacked_{ false };
	//パラメーター
	EnemyDatabase::EnemyParameter parameter_;

	//敵データベース参照
	EnemyDatabase& e_DB_{ EnemyDatabase::GetInstance() };

};
#endif//!ENEMY_H_

