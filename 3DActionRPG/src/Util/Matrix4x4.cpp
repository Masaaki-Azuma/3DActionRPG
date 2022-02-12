#include "Matrix4x4.h"
#include <cmath>
#include "Vector3.h"

using std::sin;
using std::cos;

const float EPSILON{ 0.00001f };
const float PI = 3.14159265359f;
const float Deg2Rad{ PI / 180.0f };


Matrix4x4::Matrix4x4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33):
	_00{ m00 }, _01{ m01 }, _02{ m02 }, _03{ m03 },
	_10{ m10 }, _11{ m11 }, _12{ m12 }, _13{ m13 },
	_20{ m20 }, _21{ m21 }, _22{ m22 }, _23{ m23 },
	_30{ m30 }, _31{ m31 }, _32{ m32 }, _33{ m33 }
{
}

Matrix4x4 Matrix4x4::zero()
{
	return Matrix4x4{
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f };
}

Matrix4x4 Matrix4x4::identity()
{
	return Matrix4x4{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f };
}

Matrix4x4 Matrix4x4::scale(const Vector3& v)
{
	return Matrix4x4{
		 v.x, 0.0f, 0.0f, 0.0f,
		0.0f,  v.y, 0.0f, 0.0f,
		0.0f, 0.0f , v.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f };
}

Matrix4x4 Matrix4x4::rotateX(float degree)
{
	float rad = degree * Deg2Rad;
	return Matrix4x4{
		1.0f,      0.0f,     0.0f, 0.0f,
		0.0f,  cos(rad), sin(rad), 0.0f,
		0.0f, -sin(rad), cos(rad), 0.0f,
		0.0f,      0.0f,     0.0f, 1.0f };
}

Matrix4x4 Matrix4x4::rotateY(float degree)
{
	float rad = degree * Deg2Rad;
	return Matrix4x4{
		cos(rad), 0.0f, -sin(rad), 0.0f,
		    0.0f, 1.0f,      0.0f, 0.0f,
		sin(rad), 0.0f,  cos(rad), 0.0f,
	        0.0f, 0.0f,      0.0f, 1.0f };
}

Matrix4x4 Matrix4x4::rotateZ(float degree)
{
	float rad = degree * Deg2Rad;
	return Matrix4x4{
		 cos(rad), sin(rad), 0.0f, 0.0f,
		-sin(rad), cos(rad), 0.0f, 0.0f,
		     0.0f,     0.0f, 1.0f, 0.0f,
		     0.0f,     0.0f, 0.0f, 1.0f };
}

Matrix4x4 Matrix4x4::translate(const Vector3& v)
{
	return Matrix4x4{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		 v.x,  v.y,  v.z, 1.0f };
}

Matrix4x4 Matrix4x4::transpose(const Matrix4x4& matrix)
{
	Matrix4x4 result;
	for (int r = 0; r < 4; ++r) {
		for (int c = 0; c < 4; ++c) {
			result.m[r][c] = matrix.m[c][r];
		}
	}
	return result;
}

//面倒すぎる
Matrix4x4 Matrix4x4::inverse(const Matrix4x4& matrix)
{
	Matrix4x4 result = zero();
	//float det =
	//	matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][2]
	//	+ matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][0]
	//	+ matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][1]
	//	- matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][0]
	//	- matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][1]
	//	- matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][2];

	////行列式が0のとき、逆行列が定義できないので零行列を返す
	//if (-EPSILON < det && det < EPSILON) return result;

	//result = 1 / det * Matrix4x4{
	//	matrix.m[1][1] * matrix.m[2][2] - matrix.m[1][2] * matrix.m[2][1],
	//	matrix.m[0][2] * matrix.m[2][1] - matrix.m[0][1] * matrix.m[2][2],
	//	matrix.m[0][1] * matrix.m[1][2] - matrix.m[0][2] * matrix.m[1][1],
	//	matrix.m[1][2] * matrix.m[2][0] - matrix.m[1][0] * matrix.m[2][2],
	//	matrix.m[0][0] * matrix.m[2][2] - matrix.m[0][2] * matrix.m[2][0],
	//	matrix.m[0][2] * matrix.m[1][0] - matrix.m[0][0] * matrix.m[1][2],
	//	matrix.m[1][0] * matrix.m[2][1] - matrix.m[1][1] * matrix.m[2][0],
	//	matrix.m[0][1] * matrix.m[2][0] - matrix.m[0][0] * matrix.m[2][1],
	//	matrix.m[0][0] * matrix.m[1][1] - matrix.m[0][1] * matrix.m[1][0] };
	return result;
}

Matrix4x4 Matrix4x4::transpose()
{
	Matrix4x4 result;
	for (int r = 0; r < 4; ++r) {
		for (int c = 0; c < 4; ++c) {
			result.m[r][c] = this->m[c][r];
		}
	}
	return result;
}

//面倒すぎる
Matrix4x4 Matrix4x4::inverse()
{
	Matrix4x4 result = zero();
	//float det =
	//	this->m[0][0] * this->m[1][1] * this->m[2][2]
	//	+ this->m[0][1] * this->m[1][2] * this->m[2][0]
	//	+ this->m[0][2] * this->m[1][0] * this->m[2][1]
	//	- this->m[0][2] * this->m[1][1] * this->m[2][0]
	//	- this->m[0][0] * this->m[1][2] * this->m[2][1]
	//	- this->m[0][1] * this->m[1][0] * this->m[2][2];

	////行列式が0のとき、逆行列が定義できないので零行列を返す
	//if (-EPSILON < det && det < EPSILON) return result;

	//result = 1 / det * Matrix3x3{
	//	this->m[1][1] * this->m[2][2] - this->m[1][2] * this->m[2][1],
	//	this->m[0][2] * this->m[2][1] - this->m[0][1] * this->m[2][2],
	//	this->m[0][1] * this->m[1][2] - this->m[0][2] * this->m[1][1],
	//	this->m[1][2] * this->m[2][0] - this->m[1][0] * this->m[2][2],
	//	this->m[0][0] * this->m[2][2] - this->m[0][2] * this->m[2][0],
	//	this->m[0][2] * this->m[1][0] - this->m[0][0] * this->m[1][2],
	//	this->m[1][0] * this->m[2][1] - this->m[1][1] * this->m[2][0],
	//	this->m[0][1] * this->m[2][0] - this->m[0][0] * this->m[2][1],
	//	this->m[0][0] * this->m[1][1] - this->m[0][1] * this->m[1][0] };
	return result;
}

const Matrix4x4 Matrix4x4::operator-() const
{
	Matrix4x4 result;
	for (int r = 0; r < 4; ++r) {
		for (int c = 0; c < 4; ++c) {
			result.m[r][c] = -this->m[r][c];
		}
	}
	return result;
}

Matrix4x4& Matrix4x4::operator+=(const Matrix4x4& other)
{
	for (int r = 0; r < 4; ++r) {
		for (int c = 0; c < 4; ++c) {
			this->m[r][c] += other.m[r][c];
		}
	}
	return *this;
}

Matrix4x4& Matrix4x4::operator-=(const Matrix4x4& other)
{
	for (int r = 0; r < 4; ++r) {
		for (int c = 0; c < 4; ++c) {
			this->m[r][c] -= other.m[r][c];
		}
	}
	return *this;
}

Matrix4x4& Matrix4x4::operator*=(float scalar)
{
	for (int r = 0; r < 4; ++r) {
		for (int c = 0; c < 4; ++c) {
			this->m[r][c] *= scalar;
		}
	}
	return *this;
}

Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& rm)
{
	Matrix4x4 result;
	for (int r = 0; r < 4; ++r) {
		for (int c = 0; c < 4; ++c) {
			result.m[r][c] =
				  this->m[r][0] * rm.m[0][c]
				+ this->m[r][1] * rm.m[1][c]
				+ this->m[r][2] * rm.m[2][c]
				+ this->m[r][3] * rm.m[3][c];
		}
	}
	*this = result;
	return *this;
}

const Matrix4x4 operator+(const Matrix4x4& m1, const Matrix4x4& m2)
{
	return Matrix4x4{ m1 } += m2;
}

const Matrix4x4 operator-(const Matrix4x4& m1, const Matrix4x4& m2)
{
	return Matrix4x4{ m1 } -= m2;
}

const Matrix4x4 operator*(const Matrix4x4& matrix, float scalar)
{
	return Matrix4x4{ matrix } *= scalar;
}

const Matrix4x4 operator*(float scalar, const Matrix4x4& matrix)
{
	return matrix * scalar;
}

const Vector3 operator*(const Vector3& lv, const Matrix4x4& rm)
{
	float x = lv.x * rm.m[0][0] + lv.y * rm.m[1][0] + lv.z * rm.m[2][0] + rm.m[3][0];
	float y = lv.x * rm.m[0][1] + lv.y * rm.m[1][1] + lv.z * rm.m[2][1] + rm.m[3][1];
	float z = lv.x * rm.m[0][2] + lv.y * rm.m[1][2] + lv.z * rm.m[2][2] + rm.m[3][2];
	return Vector3{ x, y, z };
}

const Matrix4x4 operator*(const Matrix4x4& lm, const Matrix4x4& rm)
{
	return Matrix4x4{ lm } *= rm;
}
