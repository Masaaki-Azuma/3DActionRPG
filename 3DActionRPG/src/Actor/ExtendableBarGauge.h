#ifndef EXTENDABLE_BAR_GAUGE_H_
#define EXTENDABLE_BAR_GAUGE_H_

class ExtendableBarGauge
{
public:
	ExtendableBarGauge() = default;
	ExtendableBarGauge(float left, float top, int width, int height, unsigned int frame_id, unsigned int bar_id, unsigned int barBG_id = -1);
	‾ExtendableBarGauge();
	void set_edge_width(int width);
	void div_frame();
	void clear_image();
	void extend(int current_max, int limit_max);
	void draw_gui(float curr_value) const;
private:
	void draw_frame() const;
	void draw_bar(float curr_value) const;
	void draw_barBG(float curr_value) const;

private:
	unsigned int frame_id_;
	unsigned int bar_id_;
	unsigned int barBG_id_;

	int frame_left_{ -1 };
	int frame_right_{ -1 };
	int frame_mid_{ -1 };

	int bar_left_{ -1 };
	int bar_right_{ -1 };
	int bar_mid_{ -1 };

	int barBG_left_{ -1 };
	int barBG_right_{ -1 };
	int barBG_mid_{ -1 };

	float left_;
	float top_;
	int width_;
	int height_;

	int current_max_{ 1 };
	int limit_max_{ 1 };
	int edge_width_{ 0 };
};
#endif//!EXTENDABLE_BAR_GAUGE_H_

