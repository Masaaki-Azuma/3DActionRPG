#ifndef TIMER_H_
#define TIMER_H_

#include <string>

#include "Util/Vector3.h"

class TimerUI
{
public:
	TimerUI();
	void set_color(unsigned int color_handle);
	void set_font(int font_handle);
	void reset(float time = 0.0f);
	void update(float delta_time);
	void draw_center(const float height) const;
	void draw(const Vector3& position) const;
	std::string timer_text() const;
	float get_sec() const;
	//指定時間を超えた瞬間か？
	bool has_excessed(float time) const;
	//指定時間を経過しているか？
	bool has_elapsed(float time) const;
private:
	float timer_;
	float prev_timer_;
	unsigned int color_;
	int font_;
};
#endif//!TIMER_H_

