//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include "Vector2.h"

const Vector2 Vector2::UNIT_X(1.0f, 0.0f);
const Vector2 Vector2::UNIT_Y(0.0f, 1.0f);
const Vector2 Vector2::NEGATIVE_UNIT_X(-1.0f, 0.0f);
const Vector2 Vector2::NEGATIVE_UNIT_Y(0.0f, -1.0f);
const Vector2 Vector2::INFINITE_VECTOR(Math::INFINITE_FLOAT);
const Vector2 Vector2::NEGATIVE_INFINITE_VECTOR(Math::NEGATIVE_INFINITE_FLOAT);

Vector2::Vector2():x(0.0f), y(0.0f)
{
	//EMPTY:
}
Vector2::Vector2(float inX, float inY):x(inX), y(inY)
{
	//EMPTY:
}
Vector2::Vector2(float inXY):x(inXY), y(inXY)
{
	//EMPTY:
}
const float* Vector2::getAsFloatPtr() const
{
	return reinterpret_cast<const float*>(&x);
}
void Vector2::set(float inX, float inY)
{
	x = inX;
	y = inY;
}
Vector2 operator+(const Vector2& a, const Vector2& b)
{
	return Vector2(a.x + b.x, a.y + b.y);
}
Vector2 operator-(const Vector2& a, const Vector2& b)
{
	return Vector2(a.x - b.x, a.y - b.y);
}
Vector2 operator*(const Vector2& a, const Vector2& b)
{
	return Vector2(a.x * b.x, a.y * b.y);
}
Vector2 operator*(const Vector2& vec, float scalar)
{
	return Vector2(vec.x * scalar, vec.y * scalar);
}
Vector2 operator*(float scalar, const Vector2& vec)
{
	return Vector2(vec.x * scalar, vec.y * scalar);
}
Vector2& Vector2::operator*=(float scalar)
{
	x *= scalar;
	y *= scalar;
	return *this;
}
bool Vector2::operator==(const Vector2& right)
{
	if (x == right.x)
	{
		if (y == right.y)
		{
			return true;
		}
		return false;
	}
	return false;
}
bool Vector2::operator!=(const Vector2& right)
{
	if (x != right.x)
	{
		if (y != right.y)
		{
			return true;
		}
		return false;
	}
	return false;
}
Vector2& Vector2::operator+=(const Vector2& right)
{
	x += right.x;
	y += right.y;
	return *this;
}
Vector2& Vector2::operator-=(const Vector2& right)
{
	x -= right.x;
	y -= right.y;
	return *this;
}
float Vector2::length2() const
{
	return (x * x + y * y);
}
float Vector2::length() const
{
	return (Math::sqrt(length2()));
}
void Vector2::normalize()
{
	float l = length();
	x /= l;
	y /= l;
}
Vector2 Math::normalize(const Vector2& vec)
{
	Vector2 temp = vec;
	temp.normalize();
	return temp;
}
float Math::dot(const Vector2& a, const Vector2& b)
{
	return (a.x * b.x + a.y * b.y);
}
Vector2 Math::lerp(const Vector2& a, const Vector2& b, float f)
{
	return Vector2(a + f * (b - a));
}
Vector2 Math::reflect(const Vector2& v, const Vector2& n)
{
	return v - 2.0f * dot(v, n) * n;
}