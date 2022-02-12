#ifndef SPHERE_H_
#define SPHERE_H_

#include "Vector3.h"

class Sphere
{
public:
	Sphere(float radius = 0.0f, const Vector3& center = Vector3::ZERO);
	//デバッグ用球表示
	void draw() const;
	//球同士の当たり判定
	bool is_collide(const Sphere& other) const;

public:
	float radius;   //球の半径
	Vector3 center; //球の中心
};
#endif//!SPHERE_H_

