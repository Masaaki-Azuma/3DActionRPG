#ifndef SIMPLE_TIMER_H_
#define SIMPLE_TIMER_H_

//簡易タイマー、重要なシステムの時間管理には適さない
class SimpleTimer
{
public:
	SimpleTimer();
	//タイマーを初期化
	void reset(float time = 0.0f);
	//タイマー更新
	void update(float delta_time);
	//経過時間
	float elapsed_sec() const;
	//ある状態になってから指定時間を経過しているか？
	bool has_elapsed(float time) const;
	//ある状態になってからちょうど指定時間をすぎたタイミングか？
	bool has_excessed(float time) const;
private:
	//状態タイマー
	float state_timer_{ 0.0f };
	//状態タイマー履歴
	float prev_state_timer_{ 0.0f };
};
#endif//!SIMPLE_TIMER_H_
