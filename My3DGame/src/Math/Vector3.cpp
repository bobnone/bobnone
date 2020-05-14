//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "Vector3.h"

const Vector3 Vector3::UNIT_X(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::UNIT_Y(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::UNIT_Z(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::NEGATIVE_UNIT_X(-1.0f, 0.0f, 0.0f);
const Vector3 Vector3::NEGATIVE_UNIT_Y(0.0f, -1.0f, 0.0f);
const Vector3 Vector3::NEGATIVE_UNIT_Z(0.0f, 0.0f, -1.0f);
const Vector3 Vector3::INFINITE_VECTOR(Math::INFINITE_FLOAT);
const Vector3 Vector3::NEGATIVE_INFINITE_VECTOR(Math::NEGATIVE_INFINITE_FLOAT);

Vector3::Vector3():x(0.0f), y(0.0f), z(0.0f)
{
	//EMPTY:
}
Vector3::Vector3(float inX, float inY, float inZ):x(inX), y(inY), z(inZ)
{
	//EMPTY:
}
Vector3::Vector3(Vector2 v, float inZ):x(v.x), y(v.y), z(inZ)
{
	//EMPTY:
}
Vector3::Vector3(float inXYZ):x(inXYZ), y(inXYZ), z(inXYZ)
{
	//EMPTY:
}
const float* Vector3::getAsFloatPtr() const
{
	return reinterpret_cast<const float*>(&x);
}
void Vector3::set(float inX, float inY, float inZ)
{
	x = inX;
	y = inY;
	z = inZ;
}
Vector3 operator+(const Vector3& a, const Vector3& b)
{
	return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
}
Vector3 operator-(const Vector3& a, const Vector3& b)
{
	return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
}
Vector3 operator*(const Vector3& left, const Vector3& right)
{
	return Vector3(left.x * right.x, left.y * right.y, left.z * right.z);
}
Vector3 operator/(const Vector3& left, const Vector3& right)
{
	return Vector3(left.x / right.x, left.y / right.y, left.z / right.z);
}
Vector3 operator*(const Vector3& vec, float scalar)
{
	return Vector3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
}
Vector3 operator*(float scalar, const Vector3& vec)
{
	return Vector3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
}
Vector3& Vector3::operator*=(float scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return *this;
}
bool Vector3::operator==(const Vector3& right)
{
	if (x == right.x)
	{
		if (y == right.y)
		{
			if (z == right.z)
			{
				return true;
			}
			return false;
		}
		return false;
	}
	return false;
}
bool Vector3::operator!=(const Vector3& right)
{
	if (x != right.x)
	{
		if (y != right.y)
		{
			if (z != right.z)
			{
				return true;
			}
			return false;
		}
		return false;
	}
	return false;
}
Vector3& Vector3::operator+=(const Vector3& right)
{
	x += right.x;
	y += right.y;
	z += right.z;
	return *this;
}
Vector3& Vector3::operator-=(const Vector3& right)
{
	x -= right.x;
	y -= right.y;
	z -= right.z;
	return *this;
}
float Vector3::length2() const
{
	return (x * x + y * y + z * z);
}
float Vector3::length() const
{
	return (Math::sqrt(length2()));
}
void Vector3::normalize()
{
	float l = length();
	x /= l;
	y /= l;
	z /= l;
}
Vector3 Math::normalize(const Vector3& vec)
{
	Vector3 temp = vec;
	temp.normalize();
	return temp;
}
float Math::dot(const Vector3& a, const Vector3& b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}
Vector3 Math::cross(const Vector3& a, const Vector3& b)
{
	Vector3 temp;
	temp.x = a.y * b.z - a.z * b.y;
	temp.y = a.z * b.x - a.x * b.z;
	temp.z = a.x * b.y - a.y * b.x;
	return temp;
}
Vector3 Math::lerp(const Vector3& a, const Vector3& b, float f)
{
	return Vector3(a + f * (b - a));
}
Vector3 Math::reflect(const Vector3& v, const Vector3& n)
{
	return v - 2.0f * dot(v, n) * n;
}