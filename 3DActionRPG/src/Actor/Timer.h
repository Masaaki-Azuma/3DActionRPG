#ifndef TIMER_H_
#define TIMER_H_

#include <string>

#include "Util/Vector3.h"

class Timer
{
public:
	Timer();
	void set_color(unsigned int color_handle);
	void set_font(int font_handle);
	void reset(float time = 0.0f);
	void update(float delta_time);
	void draw_center(const float height) const;
	void draw(const Vector3& position) const;
	std::string timer_text() const;
	float get_sec() const;
private:
	float timer_;
	unsigned int color_;
	int font_;
};
#endif//!TIMER_H_

