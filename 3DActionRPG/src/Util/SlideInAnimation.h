#ifndef SLIDE_IN_ANIMATION_H_
#define SLIDE_IN_ANIMATION_H_

#include <string>

#include "Util/Vector2.h"

class SlideInAnimation
{
public:
	SlideInAnimation();
	SlideInAnimation(int texture_id, int image_width, int image_height, float slide_speed, float target);
	SlideInAnimation(int texture_id, const Vector2& image_size, float slide_speed, float target);
	SlideInAnimation(const std::string& text, int font_handle, int font_size, unsigned int color, float slide_speed, float target);
	void start();
	void update(float delta_time);
	void draw() const;
	bool is_wait() const;
	bool is_running() const;
	bool is_end() const;

private:
	//アニメーション状態
	int state_;
	//表示対象モード
	int mode_;

	//画像ID
	int textue_id_{ -1 };
	//画像幅
	int width_{ 0 };
	//画像高さ
	int height_{ 0 };

	//表示文字
	std::string text_{ "" };
	//フォントハンドル
	int font_handle_{ -1 };
	//フォントサイズ
	int font_size_{ 0 };
	//文字色
	unsigned int color_{ 0 };

	//アニメーション速さ
	float speed_{ 1.0f };
	//目標位置
	float target_{ 0.0f };
	//位置
	Vector2 position_;
};
#endif//!SLIDE_IN_ANIMATION_H_

