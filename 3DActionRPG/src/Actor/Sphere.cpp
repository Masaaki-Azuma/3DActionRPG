#include "Sphere.h"
#include <DxLib.h>
#include "Util/DxConverter.h"

Sphere::Sphere(float radius, const Vector3& center):
	radius{radius},
	center{center}
{
}

void Sphere::draw() const
{
	DxLib::DrawSphere3D(DxConverter::GetVECTOR(center), radius, 4, GetColor(0, 255, 0), GetColor(0, 255, 0), false);
}

bool Sphere::is_collide(const Sphere& other) const
{
	//bool result = collision_sphere_sphere(radius, center, other.radius, other.center);

	float distance = (other.center - center).Length();
	if (distance <= radius + other.radius) return true;
	else                                   return false;
}
