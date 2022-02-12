#ifndef VECTOR3_H_
#define VECTOR3_H_

#include <array>

class Vector3
{
public:
	static const Vector3 ZERO;    //ゼロベクトル
	static const Vector3 RIGHT;   //右
	static const Vector3 LEFT;    //左
	static const Vector3 UP;      //上
	static const Vector3 DOWN;    //下
	static const Vector3 FORWARD; //前
	static const Vector3 BACK;    //後
	static const Vector3 NEGATIVE_INFINITY;  //ベクトルの最小値
	static const Vector3 POSITIVE_INFINITY;  //ベクトルの最大値
	
public:
	Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f);

	float LengthSquare() const;
	float Length() const;
	Vector3 Normalize() const;
	float Dot(const Vector3& other) const;
	Vector3 Cross(const Vector3& other) const;
	static float Dot(const Vector3& v1, const Vector3& v2);
	static Vector3 Cross(const Vector3& v1, const Vector3& v2);
	static float Angle(const Vector3& v1, const Vector3& v2);
	//地面に投影したベクトル間の成す符号付き角度を取得
	static float SignedAngleY(const Vector3& from, const Vector3& to);

	const Vector3 operator-() const;
	Vector3& operator+=(const Vector3& other);
	Vector3& operator-=(const Vector3& other);
	Vector3& operator*=(float scalar);
	Vector3& operator/=(float scalar);
	
public:
	union
	{
		struct
		{
			float x;
			float y;
			float z;
		};
		std::array<float, 3> xyz;
	};
};

const Vector3 operator+(const Vector3& v1, const Vector3& v2);
const Vector3 operator-(const Vector3& v1, const Vector3& v2);
const Vector3 operator*(const Vector3& v, float scalar);
const Vector3 operator/(const Vector3& v, float scalar);
bool operator==(const Vector3& v1, const Vector3& v2);
bool operator!=(const Vector3& v1, const Vector3& v2);
#endif//!VECTOR3_H_
