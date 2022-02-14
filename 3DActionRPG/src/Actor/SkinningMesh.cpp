#include "SkinningMesh.h"

#include <cmath>

#include "Util/Vector3.h"
#include "Util/DxConverter.h"
#include "Util/MyMath.h"
#include "AssetsManager/Mesh.h"

//DxLib由来のモデルハンドルとアニメーションのfpsを設定する
//意図的にsupposed_fps
SkinningMesh::SkinningMesh(int model_handle, float supposed_fps)
{
	store_model_handle(model_handle);
	supposed_fps_ = supposed_fps;
}

SkinningMesh::‾SkinningMesh()
{
}

void SkinningMesh::store_model_handle(int model_handle)
{
	//モデルハンドルを記憶
	model_handle_ = model_handle;
}

void SkinningMesh::update(float delta_time)
{
	/*3dmodel*/
	//アニメーションを更新
	anim_now_frame_ += delta_time * supposed_fps_;  //秒→フレーム数変換
	//ループ処理
	if (motion_loop_) {
		anim_now_frame_ = std::fmodf(anim_now_frame_, anim_total_frame_);
	}
	else {
		anim_now_frame_ = (std::min)(anim_now_frame_, anim_total_frame_);
	}
	MV1SetAttachAnimTime(model_handle_, anim_attach_index_, anim_now_frame_);
}

void SkinningMesh::draw() const
{
	MV1DrawModel(model_handle_);
}

void SkinningMesh::set_position(const VECTOR& position)
{
	MV1SetPosition(model_handle_, position);
}

void SkinningMesh::set_position(const Vector3& position)
{
	set_position(VECTOR{ position.x, position.y, position.z });
}

//モデルの向き（弧度法）を設定
void SkinningMesh::set_rotation(const VECTOR& rotation)
{
	DxLib::MV1SetRotationXYZ(model_handle_, rotation);
}

//モデルの向き（弧度法）を設定、DxLib用の型にキャスト
void SkinningMesh::set_rotation(const Vector3& rotation)
{
	DxLib::MV1SetRotationXYZ(model_handle_, DxConverter::GetVECTOR(rotation));
}

void SkinningMesh::set_scale(const Vector3& scale)
{
	DxLib::MV1SetScale(model_handle_, DxConverter::GetVECTOR(scale));
}

void SkinningMesh::change_anim(int anim_index, bool loop, bool reset)
{
	if (anim_index == motion_index_ && !reset) return;
	//ループ設定
	motion_loop_ = loop;
	//再生するアニメーション番号をキープ
	motion_index_ = anim_index;
	//前のアニメーションをデタッチ
	MV1DetachAnim(model_handle_, anim_attach_index_);
	//次のアニメーションをアタッチ
	anim_attach_index_ = MV1AttachAnim(model_handle_, anim_index);
	//アニメーションの総フレーム数を取得
	anim_total_frame_ = MV1GetAttachAnimTotalTime(model_handle_, anim_attach_index_);
	//アニメーションの現在フレーム数を初期化
	anim_now_frame_ = 0.0f;
	MV1SetAttachAnimTime(model_handle_, anim_attach_index_, anim_now_frame_);
}

//アニメーションの総再生時間を秒に直して返す
float SkinningMesh::anim_total_sec()
{
	return anim_total_frame_ / supposed_fps_;
}
