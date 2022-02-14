#ifndef FADE_H_
#define FADE_H_

class Fade
{
public:
	enum class State
	{
		WAIT,
		STOP,
		FADEIN, //フェードイン中
		FADEOUT,//フェードアウト中
		CLOSED, //フェードアウト完了
		OPENED, //フェードイン完了
	};
public:
	Fade();
	‾Fade();
	//フェード透明度の更新
	void Update();
	//描画
	void Draw() const;
	//フェードスタート
	void Start(State state);
	//待機中か？
	bool IsWaiting();
	//完全に隠されているか？
	bool IsClosed();
	//フェード中か？
	bool IsRunning();
	void Reset();
private:
	//フェードイン
	void FadeIn();
	//フェードアウト
	void FadeOut();

private:
	float alpha_; //不透明度 0.0〜1.0
	State state_; //フェード状態
	int image_handle_{ -1 };
};
#endif//!FADE_H_

