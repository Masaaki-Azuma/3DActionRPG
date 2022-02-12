#ifndef CAMERA_H_
#define CAMERA_H_

#include "Actor.h"

class IWorld;

class Camera : public Actor
{
public:
	Camera(IWorld* world);
	virtual void update(float delta_time) override;
	virtual void draw() const override;
private:
	//targetの背後を追跡
	void follow_target(float delta_time);
	//水平方向回転
	void rotate(float delta_time);

	//デバッグ用カメラ調整
	void arrange_camera();

private:
	IWorld* world_{ nullptr };
	Actor* target_{ nullptr };
};
#endif//!CAMERA_H_

