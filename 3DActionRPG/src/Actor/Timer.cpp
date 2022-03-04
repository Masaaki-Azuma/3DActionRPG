#include "Timer.h"

#include <cmath>
#include <sstream>
#include <iomanip>
#include <DxLib.h>

#include "AssetsManager/Font.h"

Timer::Timer():
	timer_{0.0f},
	color_{DxLib::GetColor(255,255,255)},
	font_{-1}
{
}

void Timer::set_color(unsigned int color_handle)
{
	color_ = color_handle;
}

void Timer::set_font(int font_handle)
{
	font_ = font_handle;
}

void Timer::reset()
{
	timer_ = 0.0f;
}

void Timer::update(float delta_time)
{
	timer_ += delta_time;
}

void Timer::draw() const
{
	//分、秒計算
	int sec = static_cast<int>(std::fmod(timer_, 60));
	int min = static_cast<int>(timer_ / 60);
	//"分:秒"表示を生成
	std::stringstream ss{};
	ss << std::setw(2) << std::setfill('0') << min << ":" <<
		std::setw(2) << std::setfill('0') << sec;
	//タイマー描画
	Font::draw_centered(20, ss.str(), color_, font_);
}

float Timer::get_sec() const
{
	return timer_;
}
