#include "Camera.h"
#include <DxLib.h>
#include "Input.h"
#include "Util/DxConverter.h"
#include "Util/MyMath.h"
#include "Util/Matrix4x4.h"
#include "IWorld.h"

const float RotateSpeed{ 1.0f };
Vector3 TargetOffset{ 0, 360, -390 };

Camera::Camera(IWorld* world)
{
	world_ = world;
	name_ = "Camera";
	tag_ = "CameraTag";
	position_ = Vector3{ 0, 500, -500 };
	rotation_ = Vector3{ 20.0f, 0.0f, 0.0f };

	Vector3 rad = rotation_ * MyMath::Deg2Rad;
	DxLib::SetCameraPositionAndAngle(DxConverter::GetVECTOR(position_), rad.x, rad.y, rad.z);
}

void Camera::update(float delta_time)
{
	follow_target(delta_time);
	rotate(delta_time);
}

void Camera::draw() const
{
	//弧度法へ変換
	Vector3 rad = rotation_ * MyMath::Deg2Rad;
	DxLib::SetCameraPositionAndAngle(DxConverter::GetVECTOR(position_), rad.x, rad.y, rad.z);
}

void Camera::follow_target(float delta_time)
{
	//プレイヤーを検索して背後に張り付く
	Actor* target = world_->find_actor("Player");
	if (!target) return;

	//arrange_camera();

	//カメラ位置をセット
	Vector3 offset = TargetOffset * Matrix4x4::rotateY(rotation_.y);

	position_ = target->position() + offset;
}

void Camera::rotate(float delta_time)
{
	//カメラ回転
	if (Input::get_button(PAD_INPUT_RIGHT)) {
		rotation_.y += RotateSpeed;
	}
	else if (Input::get_button(PAD_INPUT_LEFT)) {
		rotation_.y -= RotateSpeed;
	}
}

void Camera::arrange_camera()
{
	//ForDebug
	if (Input::get_button(PAD_INPUT_8)) { //W
		TargetOffset.y += 1.0f;
	}
	if (Input::get_button(PAD_INPUT_4)) { //A
		TargetOffset.z += 1.0f;
	}
	if (Input::get_button(PAD_INPUT_5)) { //S
		TargetOffset.y -= 1.0f;
	}
	if (Input::get_button(PAD_INPUT_6)) { //D
		TargetOffset.z -= 1.0f;
	}
}
