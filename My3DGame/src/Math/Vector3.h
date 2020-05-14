//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include "Vector2.h"

class Vector3
{
public:
	//Constants:
	static const Vector3 UNIT_X;
	static const Vector3 UNIT_Y;
	static const Vector3 UNIT_Z;
	static const Vector3 NEGATIVE_UNIT_X;
	static const Vector3 NEGATIVE_UNIT_Y;
	static const Vector3 NEGATIVE_UNIT_Z;
	static const Vector3 INFINITE_VECTOR;
	static const Vector3 NEGATIVE_INFINITE_VECTOR;
	//Variables:
	float x;
	float y;
	float z;
	//Constructor
	Vector3();
	explicit Vector3(float inX, float inY, float inZ);
	explicit Vector3(Vector2 v, float inZ);
	explicit Vector3(float inXYZ);
	//Cast to a const float pointer
	const float* getAsFloatPtr() const;
	//Set all three components in one line
	void set(float inX, float inY, float inZ);
	//Vector addition (a + b)
	friend Vector3 operator+(const Vector3& a, const Vector3& b);
	//Vector subtraction (a - b)
	friend Vector3 operator-(const Vector3& a, const Vector3& b);
	//Component-wise multiplication
	friend Vector3 operator*(const Vector3& left, const Vector3& right);
	//Component-wise division
	friend Vector3 operator/(const Vector3& left, const Vector3& right);
	//Scalar multiplication
	friend Vector3 operator*(const Vector3& vec, float scalar);
	//Scalar multiplication
	friend Vector3 operator*(float scalar, const Vector3& vec);
	//Scalar *=
	Vector3& operator*=(float scalar);
	//Vector ==
	bool operator==(const Vector3& right);
	//Vector !=
	bool operator!=(const Vector3& right);
	//Vector +=
	Vector3& operator+=(const Vector3& right);
	//Vector -=
	Vector3& operator-=(const Vector3& right);
	//Length squared of vector
	float length2() const;
	//Length of vector
	float length() const;
	//Normalize this vector
	void normalize();
};
namespace Math
{
	//Normalize the provided vector
	Vector3 normalize(const Vector3& vec);
	//Dot product between two vectors (a dot b)
	float dot(const Vector3& a, const Vector3& b);
	//Cross product between two vectors (a cross b)
	Vector3 cross(const Vector3& a, const Vector3& b);
	//Lerp from A to B by f
	Vector3 lerp(const Vector3& a, const Vector3& b, float f);
	//Reflect V about (normalized) N
	Vector3 reflect(const Vector3& v, const Vector3& n);
};