#ifndef ENEMY_H_
#define ENEMY_H_

#include <memory>

#include "Actor/Actor.h"
#include "Actor/SkinningMesh.h"
#include "AssetsManager/EnemyDatabase.h"
#include "AssetsManager/Sound.h"
#include "Actor/ExtendableBarGauge.h"

class Enemy : public Actor
{
public:
	//敵の基本状態を定義
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

	//HPバーを表示
	void draw_hp_gauge() const;
	//プレイヤーを検索
	std::shared_ptr<Actor> find_player();
	//プレイヤー方向のベクトルを取得
	Vector3 get_vec_to_player();
	//距離を取る
	Vector3 make_distance();
	//指定方向を向くような回転角度を取得
	float degree_forward(const Vector3& forward);
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

	//ForDebug:敵ごとのデバッグ表示
	virtual void draw_debug() const {};

protected:
	//アニメーションするメッシュ
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
	//連続ダメージに対するひるみ回数
	int flinch_count_{ 0 };
	//パラメーター
	EnemyDatabase::EnemyParameter parameter_;
	//HPゲージ
	ExtendableBarGauge hp_gauge_;

	//敵データベース参照
	EnemyDatabase& e_DB_{ EnemyDatabase::GetInstance() };
	//サウンド管理クラス参照
	Sound& sound_{ Sound::GetInstance() };

};
#endif//!ENEMY_H_

