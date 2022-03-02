#include "Vector2.h"
#include <cmath>
#include <limits>

const float EPSILON{ 0.001f };
const float F_INFINITY{ std::numeric_limits<float>::infinity() };

const Vector2 Vector2::ZERO{ 0.0f,  0.0f };
const Vector2 Vector2::LEFT{ -1.0f,  0.0f };
const Vector2 Vector2::RIGHT{ 1.0f,  0.0f };
const Vector2 Vector2::UP{ 0.0f, -1.0f };
const Vector2 Vector2::DOWN{ 0.0f,  1.0f };
const Vector2 Vector2::NEGATIVE_INFINITY{ -F_INFINITY, -F_INFINITY };
const Vector2 Vector2::POSITIVE_INFINITY{ F_INFINITY, F_INFINITY };


Vector2::Vector2(float x, float y) :
    x{ x }, y{ y }
{
}

//長さの2乗を求める
float Vector2::LengthSquare() const
{
    return Dot(*this);
}
//長さを求める
float Vector2::Length() const
{
    return std::sqrtf(LengthSquare());
}
//正規化したベクトルを返す
Vector2 Vector2::Normalize() const
{
    const float len = Length();
    if (len < EPSILON) {
        return ZERO;
    }
    return *this / len;
}
//内積を求める
float Vector2::Dot(const Vector2& other) const
{
    return x * other.x + y * other.y;
}
//外積を求める
float Vector2::Cross(const Vector2& other) const
{
    return x * other.y - y * other.x;
}

const Vector2 Vector2::operator-() const
{
    return Vector2{ -x, -y };
}

Vector2& Vector2::operator+=(const Vector2& other)
{
    x += other.x;
    y += other.y;
    return *this;
}

Vector2& Vector2::operator-=(const Vector2& other)
{
    x -= other.x;
    y -= other.y;
    return *this;
}

Vector2& Vector2::operator*=(float scalar)
{
    x *= scalar;
    y *= scalar;
    return *this;
}

Vector2& Vector2::operator/=(float scalar)
{
    //0除算はなにもせず返す
    if (scalar == 0.0f) return *this;
    x /= scalar;
    y /= scalar;
    return *this;
}

float Vector2::Dot(const Vector2& v1, const Vector2& v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

float Vector2::Cross(const Vector2& v1, const Vector2& v2)
{
    return v1.x * v2.y - v1.y * v2.x;
}

const Vector2 operator+(const Vector2& v1, const Vector2& v2)
{
    return Vector2{ v1 } += v2;
}

const Vector2 operator-(const Vector2& v1, const Vector2& v2)
{
    return Vector2{ v1 } -= v2;
}

const Vector2 operator*(const Vector2& v, float scalar)
{
    return Vector2{ v } *= scalar;
}

const Vector2 operator/(const Vector2& v, float scalar)
{
    return Vector2{ v } /= scalar;
}

bool operator==(const Vector2& v1, const Vector2& v2)
{
     if (v1.x != v2.x) return false;
     if (v2.y != v2.y) return false;
     return true;
}

bool operator!=(const Vector2& v1, const Vector2& v2)
{
    return !(v1 == v2);
}
