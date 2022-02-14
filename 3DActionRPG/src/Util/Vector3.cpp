#include "Vector3.h"

#include <cmath>
#include <limits>

const float EPSILON{ 0.001f };
const float F_INFINITY{ std::numeric_limits<float>::infinity() };

const Vector3 Vector3::ZERO   {  0.0f,  0.0f,  0.0f };
const Vector3 Vector3::RIGHT  {  1.0f,  0.0f,  0.0f };
const Vector3 Vector3::LEFT   { -1.0f,  0.0f,  0.0f };
const Vector3 Vector3::UP     {  0.0f,  1.0f,  0.0f };
const Vector3 Vector3::DOWN   {  0.0f, -1.0f,  0.0f };
const Vector3 Vector3::FORWARD{  0.0f,  0.0f,  1.0f };
const Vector3 Vector3::BACK   {  0.0f,  0.0f, -1.0f };
const Vector3 Vector3::NEGATIVE_INFINITY{ -F_INFINITY, -F_INFINITY, -F_INFINITY };
const Vector3 Vector3::POSITIVE_INFINITY{ F_INFINITY, F_INFINITY, F_INFINITY };


Vector3::Vector3(float x, float y, float z):
	x{ x }, y{ y }, z{ z }
{
}

float Vector3::LengthSquare() const
{
	return Dot(*this);
}

float Vector3::Length() const
{
	return std::sqrtf(LengthSquare());
}

Vector3 Vector3::Normalize() const
{
	float len = Length();
	if (len < EPSILON) {
		return ZERO;
	}
	return *this / len;
}

float Vector3::Dot(const Vector3& other) const
{
	return x * other.x + y * other.y + z * other.z;
}

Vector3 Vector3::Cross(const Vector3& other) const
{
	return Vector3{ y * other.z - z * other.y , z * other.x - x * other.z, x * other.y - y * other.x };
}

float Vector3::Dot(const Vector3& v1, const Vector3& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vector3 Vector3::Cross(const Vector3& v1, const Vector3& v2)
{
	return Vector3{ v1.y * v2.z - v1.z * v2.y , v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x };
}

float Vector3::Angle(const Vector3& v1, const Vector3& v2)
{
	//-1.0〜1.0
	float cos = Dot(v1.Normalize(), v2.Normalize());
	//0〜π
	return std::acosf(cos);
}

float Vector3::SignedAngleY(const Vector3& from, const Vector3& to)
{
	Vector3 fromXZ = from;
	fromXZ.y = 0.0f;
	Vector3 toXZ = to;
	toXZ.y = 0.0f;
	float angle = Angle(fromXZ, toXZ);
	if (Cross(fromXZ, toXZ).y < 0) angle = -angle;
	return angle;
}

const Vector3 Vector3::operator-() const
{
	return Vector3{ -x, -y, -z };
}

Vector3& Vector3::operator+=(const Vector3& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}

Vector3& Vector3::operator*=(float scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return *this;
}

Vector3& Vector3::operator/=(float scalar)
{
	if (scalar == 0.0f) return *this;
	x /= scalar;
	y /= scalar;
	z /= scalar;
	return *this;
}

const Vector3 operator+(const Vector3& v1, const Vector3& v2)
{
	return Vector3{ v1 } += v2;
}

const Vector3 operator-(const Vector3& v1, const Vector3& v2)
{
	return Vector3{ v1 } -= v2;
}

const Vector3 operator*(const Vector3& v, float scalar)
{
	return Vector3{ v } *= scalar;
}

const Vector3 operator/(const Vector3& v, float scalar)
{
	return Vector3{ v } /= scalar;
}

bool operator==(const Vector3& v1, const Vector3& v2)
{
	if (v1.x != v2.x) return false;
	if (v1.y != v2.y) return false;
	if (v1.z != v2.z) return false;
	return true;
}

bool operator!=(const Vector3& v1, const Vector3& v2)
{
	return !(v1 == v2);
}
