#ifndef FIELD_H_
#define FIELD_H_

#include <DxLib.h>

class Sphere;
class Vector3;

//ステージの描画や当たり判定を扱う
class Field
{
public:
	Field() = default;
	Field(int mesh_handle, int collider_handle, int skybox_handle);
	void init(int mesh_handle, int collider_handle, int skybox_handle);
	void draw() const;
	void clear();
	Vector3& collide_sphere(const Sphere& sphere, Vector3& position);

private:
	int mesh_handle_;
	int collider_handle_;
	int skybox_handle_;

	DxLib::MV1_COLL_RESULT_POLY_DIM poly_hit_info_;
};
#endif//!FIELD_H_
