#include "ExtendableBarGauge.h"

#include <DxLib.h>

#include "AssetsManager/Image.h"

ExtendableBarGauge::ExtendableBarGauge(unsigned int frame_id, unsigned int bar_id, float left, float top, int width, int height):
	frame_id_{ frame_id },
	bar_id_{ bar_id },
	left_{ left },
	top_{ top },
	width_{ width },
	height_{ height }
{
	
}

ExtendableBarGauge::‾ExtendableBarGauge()
{
	clear_frame();
}

void ExtendableBarGauge::set_edge_width(int width)
{
	edge_width_ = width;
	div_frame();
}

void ExtendableBarGauge::div_frame()
{
	clear_frame();
	frame_left_ = DxLib::DerivationGraph(0, 0, edge_width_, height_, Image::texture_handle(frame_id_));
	frame_right_ = DxLib::DerivationGraph(width_ - edge_width_, 0, edge_width_, height_, Image::texture_handle(frame_id_));
	frame_mid_ = DxLib::DerivationGraph(edge_width_, 0, width_ - edge_width_ * 2, height_, Image::texture_handle(frame_id_));

	bar_left_ = DxLib::DerivationGraph(0, 0, edge_width_, height_, Image::texture_handle(bar_id_));
	bar_right_ = DxLib::DerivationGraph(width_ - edge_width_, 0, edge_width_, height_, Image::texture_handle(bar_id_));
	bar_mid_ = DxLib::DerivationGraph(edge_width_, 0, width_ - edge_width_ * 2, height_, Image::texture_handle(bar_id_));

}

void ExtendableBarGauge::clear_frame()
{
	DxLib::DeleteGraph(frame_left_);
	DxLib::DeleteGraph(frame_right_);
	DxLib::DeleteGraph(frame_mid_);
	DxLib::DeleteGraph(bar_left_);
	DxLib::DeleteGraph(bar_right_);
	DxLib::DeleteGraph(bar_mid_);
}

void ExtendableBarGauge::extend(int current_max, int limit_max)
{
	current_max_ = current_max;
	limit_max_ = limit_max;
}

void ExtendableBarGauge::draw_gui(float curr_value) const
{
	/*ゲージ中身*/
	//左端
	DxLib::DrawGraphF(left_, top_, bar_left_, true);
	//間
	int curr_width = static_cast<int>(width_ * curr_value / limit_max_);
	DxLib::DrawExtendGraphF(left_ + edge_width_, top_, left_ + curr_width - edge_width_, top_ + height_, bar_mid_, true);
	//右端
	DxLib::DrawGraphF(left_ + curr_width - edge_width_, top_, bar_right_, true);

	/*ゲージ枠*/
	//左端
	DxLib::DrawGraphF(left_, top_, frame_left_, true);
	//間
	int curr_frame_width = static_cast<int>(width_ * current_max_ / limit_max_);
	DxLib::DrawExtendGraphF(left_ + edge_width_, top_, left_ + curr_frame_width - edge_width_, top_ + height_, frame_mid_, true);
	//右端
	DxLib::DrawGraphF(left_ + curr_frame_width - edge_width_, top_, frame_right_, true);
}
