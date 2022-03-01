#include "BarGauge.h"
#include "AssetsManager/Image.h"

BarGauge::BarGauge(unsigned int frame_id, unsigned int bar_id, float left, float top, int width, int height):
	frame_id_{frame_id},
	bar_id_{bar_id},
	left_{left},
	top_{top},
	width_{width},
	height_{height}
{

}

void BarGauge::draw_gui(int base_value, int curr_value) const
{
	int curr_width = width_ * curr_value / base_value;

	DxLib::DrawRectGraphF(left_, top_, 0, 0, curr_width, 40, Image::texture_handle(bar_id_), true);

	Image::draw_graph(Texture_GaugeFrame, left_, top_);
}
