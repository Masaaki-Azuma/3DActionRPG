#ifndef FADE_IN_ANIMATION_H_
#define FADE_IN_ANIMATION_H_

#include "Util/Vector3.h"

class FadeInAnimation
{
public:
	FadeInAnimation(int texture_id, const Vector3& position, float distance);
	void start();
	void reset();
	void update(float delta_time);
	void draw() const;
	bool is_waiting() const;
	bool is_running() const;
	bool is_end() const;

private:
	int texture_;
	Vector3 position_;
	Vector3 initial_pos_;
	float distance_;
	float alpha_;
	int state_;
};
#endif//!FADE_IN_ANIMATION_H_
