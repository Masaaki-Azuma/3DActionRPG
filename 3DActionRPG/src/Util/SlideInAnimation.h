#ifndef SLIDE_IN_ANIMATION_H_
#define SLIDE_IN_ANIMATION_H_

#include "Util/Vector2.h"

class SlideInAnimation
{
public:
	SlideInAnimation();
	SlideInAnimation(int texture_id, int image_width, int image_height, float slide_speed, float target);
	SlideInAnimation(int texture_id, const Vector2& image_size, float slide_speed, float target);
	void start();
	void update(float delta_time);
	void draw() const;
	bool is_wait() const;
	bool is_running() const;
	bool is_end() const;

private:
	//画像ID
	int textue_id_;
	//画像幅
	int width_;
	//画像高さ
	int height_;
	//アニメーション速さ
	float speed_{ 1.0f };
	//目標位置
	float target_;
	//位置
	Vector2 position_;
	//アニメーション状態
	int state_;
};
#endif//!SLIDE_IN_ANIMATION_H_

