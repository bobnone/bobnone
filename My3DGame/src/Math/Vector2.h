//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include "Functions.h"

class Vector2
{
public:
	//Constants:
	static const Vector2 UNIT_X;
	static const Vector2 UNIT_Y;
	static const Vector2 NEGATIVE_UNIT_X;
	static const Vector2 NEGATIVE_UNIT_Y;
	static const Vector2 INFINITE_VECTOR;
	static const Vector2 NEGATIVE_INFINITE_VECTOR;
	//Variables:
	float x;
	float y;
	//Constructor
	Vector2();
	explicit Vector2(float inX, float inY);
	explicit Vector2(float inXY);
	const float* getAsFloatPtr() const;
	//Set both components in one line
	void set(float inX, float inY);
	//Vector addition (a + b)
	friend Vector2 operator+(const Vector2& a, const Vector2& b);
	//Vector subtraction (a - b)
	friend Vector2 operator-(const Vector2& a, const Vector2& b);
	//Component-wise multiplication (a.x * b.x, ...)
	friend Vector2 operator*(const Vector2& a, const Vector2& b);
	//Scalar multiplication
	friend Vector2 operator*(const Vector2& vec, float scalar);
	//Scalar multiplication
	friend Vector2 operator*(float scalar, const Vector2& vec);
	//Scalar *=
	Vector2& operator*=(float scalar);
	//Vector ==
	bool operator==(const Vector2& right);
	//Vector !=
	bool operator!=(const Vector2& right);
	//Vector +=
	Vector2& operator+=(const Vector2& right);
	//Vector -=
	Vector2& operator-=(const Vector2& right);
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
	Vector2 normalize(const Vector2& vec);
	//Dot product between two vectors (a dot b)
	float dot(const Vector2& a, const Vector2& b);
	//Lerp from A to B by f
	Vector2 lerp(const Vector2& a, const Vector2& b, float f);
	//Reflect V about (normalized) N
	Vector2 reflect(const Vector2& v, const Vector2& n);
};