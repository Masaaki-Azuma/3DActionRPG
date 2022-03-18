#include "ExtendableBarGauge.h"

#include <DxLib.h>

#include "AssetsManager/Image.h"

ExtendableBarGauge::ExtendableBarGauge(float left, float top, int width, int height, int frame_id, int bar_id, int barBG_id):
	left_{ left },
	top_{ top },
	width_{ width },
	height_{ height },
	frame_id_{ frame_id },
	bar_id_{ bar_id },
	barBG_id_{barBG_id}
{
}

ExtendableBarGauge::‾ExtendableBarGauge()
{
	clear_image();
}

void ExtendableBarGauge::set_edge_width(int width)
{
	edge_width_ = width;
	div_frame();
}

void ExtendableBarGauge::div_frame()
{
	Image& image = Image::GetInstance();
	//管理中の分割画像を破棄
	clear_image();
	//ゲージ枠画像を分割
	frame_left_ = DxLib::DerivationGraph(0, 0, edge_width_, height_, image.texture_handle(frame_id_));
	frame_right_ = DxLib::DerivationGraph(width_ - edge_width_, 0, edge_width_, height_, image.texture_handle(frame_id_));
	frame_mid_ = DxLib::DerivationGraph(edge_width_, 0, width_ - edge_width_ * 2, height_, image.texture_handle(frame_id_));
	//ゲージバー画像を分割
	bar_left_ = DxLib::DerivationGraph(0, 0, edge_width_, height_, image.texture_handle(bar_id_));
	bar_right_ = DxLib::DerivationGraph(width_ - edge_width_, 0, edge_width_, height_, image.texture_handle(bar_id_));
	bar_mid_ = DxLib::DerivationGraph(edge_width_, 0, width_ - edge_width_ * 2, height_, image.texture_handle(bar_id_));
	//ゲージバー背景画像を分割
	if (barBG_id_ != -1) {
		barBG_left_ = DxLib::DerivationGraph(0, 0, edge_width_, height_, image.texture_handle(barBG_id_));
		barBG_right_ = DxLib::DerivationGraph(width_ - edge_width_, 0, edge_width_, height_, image.texture_handle(barBG_id_));
		barBG_mid_ = DxLib::DerivationGraph(edge_width_, 0, width_ - edge_width_ * 2, height_, image.texture_handle(barBG_id_));
	}
}

void ExtendableBarGauge::clear_image()
{
	DxLib::DeleteGraph(frame_left_);
	DxLib::DeleteGraph(frame_right_);
	DxLib::DeleteGraph(frame_mid_);
	DxLib::DeleteGraph(bar_left_);
	DxLib::DeleteGraph(bar_right_);
	DxLib::DeleteGraph(bar_mid_);
	DxLib::DeleteGraph(barBG_left_);
	DxLib::DeleteGraph(barBG_right_);
	DxLib::DeleteGraph(barBG_mid_);

	frame_left_ = -1;
	frame_right_ = -1;
	frame_mid_ = -1;
	bar_left_ = -1;
	bar_right_ = -1;
	bar_mid_ = -1;
	barBG_left_ = -1;
	barBG_right_ = -1;
	barBG_mid_ = -1;
}

void ExtendableBarGauge::extend(int current_max, int limit_max)
{
	current_max_ = current_max;
	limit_max_ = limit_max;
}

void ExtendableBarGauge::draw_gui(float curr_value) const
{
	/*ゲージ背景*/
	draw_barBG(curr_value);
	/*ゲージ中身*/
	draw_bar(curr_value);
	/*ゲージ枠*/
	draw_frame();
}

void ExtendableBarGauge::draw_frame() const
{
	//左端
	DxLib::DrawGraphF(left_, top_, frame_left_, true);
	//間
	int curr_frame_width = static_cast<int>((width_ - edge_width_ * 2) * current_max_ / limit_max_);

	DxLib::DrawExtendGraphF(left_ + edge_width_, top_, left_ + edge_width_ + curr_frame_width, top_ + height_, frame_mid_, true);
	//右端
	DxLib::DrawGraphF(left_ + edge_width_ + curr_frame_width, top_, frame_right_, true);
}

void ExtendableBarGauge::draw_bar(float curr_value) const
{
	int curr_width = static_cast<int>((width_ - edge_width_ * 2) * curr_value / limit_max_);
	DxLib::DrawExtendGraphF(left_ + edge_width_, top_, left_ + edge_width_ + curr_width, top_ + height_, bar_mid_, true);
}

void ExtendableBarGauge::draw_barBG(float curr_value) const
{
	if (barBG_id_ != -1) {
		//間
		int curr_width = static_cast<int>((width_ - edge_width_ * 2) * current_max_ / limit_max_);
		DxLib::DrawExtendGraphF(left_ + edge_width_, top_, left_ + edge_width_ + curr_width, top_ + height_, barBG_mid_, true);
	}
}
