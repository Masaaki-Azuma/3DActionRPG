#ifndef BAR_GAUGE_H_
#define BAR_GAUGE_H_

class BarGauge
{
public:
	BarGauge() = default;
	BarGauge(unsigned int frame_id, unsigned int bar_id, float left, float top, int width, int height);

	void draw_gui(int base_value, int curr_value) const;

private:
	unsigned int frame_id_;
	unsigned int bar_id_;
	float left_;
	float top_;
	int width_;
	int height_;
};
#endif//!BAR_GAUGE_H_

