#ifndef VECTOR2_H_
#define VECTOR2_H_

#include <array>

class Vector2
{
public:
	static const Vector2 ZERO;  //ゼロベクトル
	static const Vector2 LEFT;  //左
	static const Vector2 RIGHT; //右
	static const Vector2 UP;    //上
	static const Vector2 DOWN;  //下
	static const Vector2 NEGATIVE_INFINITY;  //ベクトルの最小値
	static const Vector2 POSITIVE_INFINITY;  //ベクトルの最大値
public:
	Vector2(float x = 0.0f, float y = 0.0f);

	float LengthSquare() const;
	float Length() const;
	Vector2 Normalize() const;
	float Dot(const Vector2& other) const;
	float Cross(const Vector2& other) const;

	static float Dot(const Vector2& v1, const Vector2& v2);
	static float Cross(const Vector2& v1, const Vector2& v2);

	const Vector2 operator-() const;
	//複合代入演算子はメンバ関数でなきゃいけないらしい
	//既存のインスタンスを変化させる
	Vector2& operator+=(const Vector2& other);
	Vector2& operator-=(const Vector2& other);
	Vector2& operator*=(float scalar);
	Vector2& operator/=(float scalar);
	//クラス内の演算子オーバーロードでは、同じ型同士の計算しかできない(?)

public:
	union
	{
		struct
		{
			float x;
			float y;
		};
		std::array<float, 2> xy;
	};
};

//代入を含まない四則演算子オーバーロードは、グローバル関数
//新たなインスタンスを戻り値とする
const Vector2 operator+(const Vector2& v1, const Vector2& v2);
const Vector2 operator-(const Vector2& v1, const Vector2& v2);
const Vector2 operator*(const Vector2& v, float scalar);
const Vector2 operator/(const Vector2& v, float scalar);
bool operator==(const Vector2& v1, const Vector2& v2);
bool operator!=(const Vector2& v1, const Vector2& v2);


#endif//!VECTOR2_H_