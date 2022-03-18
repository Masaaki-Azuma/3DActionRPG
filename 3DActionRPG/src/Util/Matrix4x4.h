#ifndef MATRIX_4X4_H_
#define MATRIX_4X4_H_

class Vector3;

class Matrix4x4
{
public:
	Matrix4x4() = default;
	Matrix4x4(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33);
	static Matrix4x4 zero();
	static Matrix4x4 identity();
	static Matrix4x4 scale(const Vector3& v);
	static Matrix4x4 rotateX(float degree);
	static Matrix4x4 rotateY(float degree);
	static Matrix4x4 rotateZ(float degree);
	static Matrix4x4 translate(const Vector3& v);
	static Matrix4x4 transpose(const Matrix4x4& matrix);
	static Matrix4x4 inverse(const Matrix4x4& matrix);

	Matrix4x4 transpose();
	Matrix4x4 inverse();

	const Matrix4x4 operator-() const;
	Matrix4x4& operator+=(const Matrix4x4& other);
	Matrix4x4& operator-=(const Matrix4x4& other);
	Matrix4x4& operator*=(float scalar);
	Matrix4x4& operator*=(const Matrix4x4& rm);

public:
	union
	{
		struct
		{
			float _00, _01, _02, _03;
			float _10, _11, _12, _13;
			float _20, _21, _22, _23;
			float _30, _31, _32, _33;
		};
		float m[4][4];
		float v[16];
	};
};

const Matrix4x4 operator+(const Matrix4x4& m1, const Matrix4x4& m2);
const Matrix4x4 operator-(const Matrix4x4& m1, const Matrix4x4& m2);
const Matrix4x4 operator*(const Matrix4x4& matrix, float scalar);
const Matrix4x4 operator*(float scalar, const Matrix4x4& matrix);
const Vector3 operator*(const Vector3& lv, const Matrix4x4& rm);
const Matrix4x4 operator*(const Matrix4x4& lm, const Matrix4x4& rm);
#endif//!MATRIX_4X4_H_