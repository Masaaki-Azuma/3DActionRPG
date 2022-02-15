#include "Fade.h"

#include <DxLib.h>

const float FadeSpeed{ 0.04f };

Fade::Fade():
	alpha_(0),
	state_(State::WAIT)
{
	image_handle_ = DxLib::LoadGraph("Assets/Image/black.png");
}

Fade::‾Fade()
{
	DxLib::DeleteGraph(image_handle_);
	image_handle_ = -1;
}

//不透明度を徐々に下げる
void Fade::FadeIn()
{
	alpha_ -= FadeSpeed;
	//完全にフェードインするために少しオーバーラン
	if (alpha_ <= 0.0f - FadeSpeed) {
		alpha_ = 0.0f;
		state_ = State::OPENED;
	}
}

//不透明度を徐々に上げる
void Fade::FadeOut()
{
	alpha_ += FadeSpeed;
	//完全にフェードインするために少しオーバーラン
	if (alpha_ >= 1.0f + FadeSpeed) {
		alpha_ = 1.0f;
		state_ = State::CLOSED;
	}
}

void Fade::Update()
{
	//状態に応じてフェード処理
	switch (state_) {
	case State::WAIT:                       break;
	case State::FADEIN:      FadeIn();      break;
	case State::FADEOUT:     FadeOut();     break;
	}
}

void Fade::Draw() const
{
	if (state_ == State::WAIT) return;

	//0.0〜1.0を0〜255へ変換
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(alpha_ * 255));
	DxLib::DrawGraph(0, 0, image_handle_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void Fade::Start(State state)
{
	if (state == State::FADEIN) {
		alpha_ = 1.0f;
		state_ = state;
	}
	else if (state == State::FADEOUT) {
		alpha_ = 0.0f;
		state_ = state;
	}
}

bool Fade::IsWaiting()
{
	return state_ == State::OPENED || state_ == State::CLOSED;
}

bool Fade::IsClosed()
{
	return state_ == State::CLOSED;
}

bool Fade::IsRunning()
{
	return !IsWaiting();
}

void Fade::Reset()
{
	state_ = State::WAIT;
}
