#ifndef PLAYER_H_
#define PLAYER_H_

#include "Actor.h"
#include "SkinningMesh.h"
#include "Util/PadInput.h"
#include "AssetsManager/PlayerDatabase.h"
#include "AssetsManager/Sound.h"

class Sound;

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
	Player(IWorld* world, const Vector3& position);
	‾Player();
	virtual void update(float delta_time) override;
	virtual void draw() const override;
	virtual void react(Actor& other) override;
	bool store_model_handle(int model_handle);
private:
	//
	void update_state(float delta_time);
	//状態を変化させる
	void change_state(State state, unsigned int motion, bool loop = true, bool motion_interruption = false);
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
	//指定時間が経過していた時に攻撃判定を生成
	void timely_generate_attack(float time);

	//コンボ攻撃
	void combo_attack(unsigned motion, float lifespan, float delay = 0.0f);
	//ダメージを受ける
	void take_damage(int damage);
	//状態タイマー更新
	void update_state_timer_(float delta_time);
	//ある状態になってから指定時間を経過しているか？
	bool has_elapsed(float time);
	//ある状態になってからちょうど指定時間をすぎたタイミングか？
	bool has_excessed(float time);
	//モーションが終了したか？
	bool is_motion_end() const;

	Vector3 camera_angle();
	Vector3 camera_forward();

private:
	//アニメーションメッシュ
	SkinningMesh  mesh_;
	//状態
	State state_;
	//状態タイマー
	float state_timer_;
	//状態タイマー履歴
	float prev_state_timer_;
	//モーション番号
	unsigned int motion_;
	//モーションループするか？
	bool motion_loop_;
	//モーション割込み
	bool motion_interruption_;
	//コンボカウンター
	int combo_counter_{ 0 };
	//戦闘パラメーター
	PlayerDatabase::Parameter parameter_;

	//プレイヤーデータ参照
	PlayerDatabase& p_db_{ PlayerDatabase::GetInstance() };
	//サウンド管理クラス参照
	Sound& sound_{ Sound::GetInstance() };
	//入力管理クラス参照
	PadInput& input_{ PadInput::GetInstance() };

};
#endif//!PLAYER_H_

