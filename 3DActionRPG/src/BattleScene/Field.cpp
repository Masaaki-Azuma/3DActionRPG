#include "Field.h"

#include "Actor/Sphere.h"
#include "Util/Vector3.h"
#include "Util/DxConverter.h"

Field::Field(int mesh_handle, int collider_handle, int skybox_handle)
{
	init(mesh_handle, collider_handle, skybox_handle);
}

void Field::init(int mesh_handle, int collider_handle, int skybox_handle)
{
	mesh_handle_ = mesh_handle;
	collider_handle_ = collider_handle;
	skybox_handle_ = skybox_handle;
}

void Field::draw() const
{
	//スカイボックスの描画
	DxLib::SetUseLighting(FALSE);
	DxLib::MV1DrawModel(skybox_handle_);
	DxLib::SetUseLighting(TRUE);

	//ステージメッシュの描画
	DxLib::MV1DrawModel(mesh_handle_);
}

void Field::clear()
{
	//ステージの衝突情報の破棄
	DxLib::MV1TerminateCollInfo(mesh_handle_);
	//ハンドルをリセット
	mesh_handle_ = -1;
	collider_handle_ = -1;
	skybox_handle_ = -1;
}

Vector3& Field::collide_sphere(const Sphere& sphere, Vector3& position)
{
	//壁から押し戻し
	poly_hit_info_ = DxLib::MV1CollCheck_Sphere(collider_handle_, -1, DxConverter::GetVECTOR(sphere.center), sphere.radius);
	if (poly_hit_info_.HitNum >= 1) {
		//当たっているポリゴンの数だけ繰り返す
		for (int i = 0; i < poly_hit_info_.HitNum; ++i) {
			//交点
			Vector3 intersect = DxConverter::GetVector3(poly_hit_info_.Dim[i].HitPosition);
			//ヒットポリゴンの法線
			Vector3 normal = DxConverter::GetVector3(poly_hit_info_.Dim[i].Normal);
			//押し戻し距離
			float distance = sphere.radius - (intersect - sphere.center).Length();
			position += normal * distance;
		}
	}
	//ポリゴンヒット情報を削除
	DxLib::MV1CollResultPolyDimTerminate(poly_hit_info_);
	return position;
}
