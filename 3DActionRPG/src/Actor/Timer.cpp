#include "Timer.h"

#include <cmath>
#include <sstream>
#include <iomanip>
#include <DxLib.h>

#include "AssetsManager/Font.h"

Timer::Timer():
	timer_{0.0f},
	prev_timer_{0.0f},
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

void Timer::reset(float time)
{
	timer_ = time;
	prev_timer_ = time;
}

void Timer::update(float delta_time)
{
	prev_timer_ = timer_;
	timer_ += delta_time;
}

void Timer::draw_center(const float height) const
{
	//タイマー描画
	Font::draw_centered(height, timer_text(), color_, font_);
}

void Timer::draw(const Vector3& position) const
{
	//タイマー描画
	Font::draw(position.x, position.y, timer_text(), color_, font_);
}

//"分:秒"形式の文字列を返す
std::string Timer::timer_text() const
{
	int sec = static_cast<int>(std::fmod(timer_, 60));
	int min = static_cast<int>(timer_ / 60);
	//"分:秒"表示を生成
	std::stringstream ss{};
	ss << std::setw(2) << std::setfill('0') << min << ":" <<
		std::setw(2) << std::setfill('0') << sec;
	return ss.str();
}

float Timer::get_sec() const
{
	return timer_;
}

bool Timer::has_excessed(float time) const
{
	return timer_ >= time && prev_timer_ < time;
}

bool Timer::has_elapsed(float time) const
{
	return timer_ >= time;
}
