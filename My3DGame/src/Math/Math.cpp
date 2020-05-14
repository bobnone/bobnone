//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include "Math.h"

Vector2 Math::transform(const Vector2& v, const Matrix3x3& m, float w)
{
	Vector2 retVal;
	retVal.x = v.x * m.matrix[0][0] + v.y * m.matrix[1][0] + w * m.matrix[2][0];
	retVal.y = v.x * m.matrix[0][1] + v.y * m.matrix[1][1] + w * m.matrix[2][1];
	//ignore w since we aren't returning a new value for it...
	return retVal;
}
Vector3 Math::transform(const Vector3& v, const Matrix4x4& m, float w)
{
	Vector3 retVal;
	retVal.x = v.x * m.matrix[0][0] + v.y * m.matrix[1][0] + v.z * m.matrix[2][0] + w * m.matrix[3][0];
	retVal.y = v.x * m.matrix[0][1] + v.y * m.matrix[1][1] + v.z * m.matrix[2][1] + w * m.matrix[3][1];
	retVal.z = v.x * m.matrix[0][2] + v.y * m.matrix[1][2] + v.z * m.matrix[2][2] + w * m.matrix[3][2];
	//ignore w since we aren't returning a new value for it...
	return retVal;
}
Vector3 Math::transform(const Vector3& v, const Quaternion& q)
{
	//v + 2.0*cross(q.xyz, cross(q.xyz,v) + q.w*v);
	Vector3 qv(q.x, q.y, q.z);
	Vector3 retVal = v;
	retVal += 2.0f * cross(qv, cross(qv, v) + q.w * v);
	return retVal;
}
Vector3 Math::transformWithPerspDiv(const Vector3& v, const Matrix4x4& m, float w)
{
	Vector3 retVal;
	retVal.x = v.x * m.matrix[0][0] + v.y * m.matrix[1][0] + v.z * m.matrix[2][0] + w * m.matrix[3][0];
	retVal.y = v.x * m.matrix[0][1] + v.y * m.matrix[1][1] + v.z * m.matrix[2][1] + w * m.matrix[3][1];
	retVal.z = v.x * m.matrix[0][2] + v.y * m.matrix[1][2] + v.z * m.matrix[2][2] + w * m.matrix[3][2];
	float transformedW = v.x * m.matrix[0][3] + v.y * m.matrix[1][3] + v.z * m.matrix[2][3] + w * m.matrix[3][3];
	if (!Math::nearZero(Math::abs(transformedW)))
	{
		transformedW = 1.0f / transformedW;
		retVal *= transformedW;
	}
	return retVal;
}