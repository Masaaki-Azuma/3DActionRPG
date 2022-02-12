#pragma once

#include <DxLib.h>

class Vector3;

class DxConverter
{
public:
	static VECTOR GetVECTOR(const Vector3& v);
	static Vector3 GetVector3(const VECTOR& v);
};

