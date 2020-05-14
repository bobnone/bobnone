//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include "Matrix4x4.h"

//Quaternion (Mathematical unit)
class Quaternion
{
public:
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 1.0f;
	Quaternion();
	// This directly sets the quaternion components -- don't use for axis/angle
	explicit Quaternion(float inX, float inY, float inZ, float inW);
	//Construct the quaternion from an axis and angle
	//Note: It is assumed that axis is already normalized, and the angle is in radians
	explicit Quaternion(const Vector3& axis, float angle);
	//Directly set the internal components
	void set(float inX, float inY, float inZ, float inW);
	void conjugate();
	float length2() const;
	float length() const;
	//Normalize the Quaternion
	void normalize();
};
namespace Math
{
	//Normalize the provided Quaternion
	Quaternion normalize(const Quaternion& q);
	//Linear interpolation
	Quaternion lerp(const Quaternion& a, const Quaternion& b, float f);
	float dot(const Quaternion& a, const Quaternion& b);
	//Spherical Linear Interpolation
	Quaternion slerp(const Quaternion& a, const Quaternion& b, float f);
	//Concatenate - rotate by q followed BY p
	Quaternion concatenate(const Quaternion& q, const Quaternion& p);
	//Create a rotation matrix from a quaternion
	Matrix4x4 toMatrix(const class Quaternion& q);
};