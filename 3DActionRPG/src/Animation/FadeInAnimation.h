#ifndef FADE_IN_ANIMATION_H_
#define FADE_IN_ANIMATION_H_

#include <string>

#include "Util/Vector3.h"

class FadeInAnimation
{
public:
	FadeInAnimation(int texture_id, const Vector3& position, float sec, float distance = 0.0f);
	FadeInAnimation(const std::string& text, int font_handle, int color, const Vector3& initial_pos, float sec, float distance = 0.0f);

	void start();
	void reset();
	void update(float delta_time);
	void draw() const;
	bool is_waiting() const;
	bool is_running() const;
	bool is_end() const;
private:
	bool has_reached_target() const;
	bool is_completely_opaque() const;
	Vector3 target_pos() const;

private:
	//アニメーション状態
	int state_;
	//描画モード
	int mode_;

	int texture_;

	std::string text_;
	int font_handle_;
	int font_color_;

	Vector3 position_;
	Vector3 prev_position_;
	Vector3 initial_pos_;
	//アニメーション時間
	float second_;
	//縦方向の移動距離
	float distance_;
	//不透明度
	float alpha_;
};
#endif//!FADE_IN_ANIMATION_H_
