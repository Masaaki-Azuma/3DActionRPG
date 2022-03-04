#ifndef TIMER_H_
#define TIMER_H_

class Timer
{
public:
	Timer();
	void set_color(unsigned int color_handle);
	void set_font(int font_handle);
	void reset();
	void update(float delta_time);
	void draw() const;
	float get_sec() const;
private:
	float timer_;
	unsigned int color_;
	int font_;
};
#endif//!TIMER_H_

