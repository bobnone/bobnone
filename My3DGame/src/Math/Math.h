//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include "Matrix3x3.h"
#include "Quaternion.h"

namespace Math
{
	//Transform a Vector2 by Matrix3
	Vector2 transform(const Vector2& v, const class Matrix3x3& m, float w = 1.0f);
	//Transform a Vector3 by a Matrix4
	Vector3 transform(const Vector3& v, const class Matrix4x4& m, float w = 1.0f);
	//Transform a Vector3 by a Quaternion
	Vector3 transform(const Vector3& v, const class Quaternion& q);
	//This will transform the vector and renormalize the w component
	Vector3 transformWithPerspDiv(const Vector3& v, const class Matrix4x4& m, float w = 1.0f);
};