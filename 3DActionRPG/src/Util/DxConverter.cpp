#include "DxConverter.h"
#include "Vector3.h"

VECTOR DxConverter::GetVECTOR(const Vector3& v)
{
	return VECTOR{ v.x, v.y, v.z };
}

Vector3 DxConverter::GetVector3(const VECTOR& v)
{
	return Vector3{ v.x, v.y,v.z };
}
