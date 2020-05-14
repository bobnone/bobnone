//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "Quaternion.h"

Quaternion::Quaternion()
{
	//EMPTY:
}
Quaternion::Quaternion(float inX, float inY, float inZ, float inW)
{
	set(inX, inY, inZ, inW);
}
Quaternion::Quaternion(const Vector3& axis, float angle)
{
	float scalar = Math::sin(angle / 2.0f);
	x = axis.x * scalar;
	y = axis.y * scalar;
	z = axis.z * scalar;
	w = Math::cos(angle / 2.0f);
}
void Quaternion::set(float inX, float inY, float inZ, float inW)
{
	x = inX;
	y = inY;
	z = inZ;
	w = inW;
}
void Quaternion::conjugate()
{
	x *= -1.0f;
	y *= -1.0f;
	z *= -1.0f;
}
float Quaternion::length2() const
{
	return (x * x + y * y + z * z + w * w);
}
float Quaternion::length() const
{
	return Math::sqrt(length2());
}
void Quaternion::normalize()
{
	float l = length();
	x /= l;
	y /= l;
	z /= l;
	w /= l;
}
Quaternion Math::normalize(const Quaternion& q)
{
	Quaternion retVal = q;
	retVal.normalize();
	return retVal;
}
Quaternion Math::lerp(const Quaternion& a, const Quaternion& b, float f)
{
	Quaternion retVal;
	retVal.x = lerp(a.x, b.x, f);
	retVal.y = lerp(a.y, b.y, f);
	retVal.z = lerp(a.z, b.z, f);
	retVal.w = lerp(a.w, b.w, f);
	retVal.normalize();
	return retVal;
}
float Math::dot(const Quaternion& a, const Quaternion& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}
Quaternion Math::slerp(const Quaternion& a, const Quaternion& b, float f)
{
	float rawCosm = dot(a, b);
	float cosom = -rawCosm;
	if(rawCosm >= 0.0f)
	{
		cosom = rawCosm;
	}
	float scale0, scale1;
	if(cosom < 0.9999f)
	{
		const float omega = acos(cosom);
		const float invSin = 1.f / sin(omega);
		scale0 = sin((1.f - f) * omega) * invSin;
		scale1 = sin(f * omega) * invSin;
	}
	else
	{
		//Use linear interpolation if the quaternions are collinear
		scale0 = 1.0f - f;
		scale1 = f;
	}
	if(rawCosm < 0.0f)
	{
		scale1 = -scale1;
	}
	Quaternion retVal;
	retVal.x = scale0 * a.x + scale1 * b.x;
	retVal.y = scale0 * a.y + scale1 * b.y;
	retVal.z = scale0 * a.z + scale1 * b.z;
	retVal.w = scale0 * a.w + scale1 * b.w;
	retVal.normalize();
	return retVal;
}
Quaternion Math::concatenate(const Quaternion& q, const Quaternion& p)
{
	Quaternion retVal;
	//Vector component is:
	//ps * qv + qs * pv + pv x qv
	Vector3 qv(q.x, q.y, q.z);
	Vector3 pv(p.x, p.y, p.z);
	Vector3 newVec = p.w * qv + q.w * pv + cross(pv, qv);
	retVal.x = newVec.x;
	retVal.y = newVec.y;
	retVal.z = newVec.z;
	//Scalar component is:
	//ps * qs - pv . qv
	retVal.w = p.w * q.w - dot(pv, qv);
	return retVal;
}
Matrix4x4 Math::toMatrix(const class Quaternion& q)
{
	float matrix[4][4];
	matrix[0][0] = 1.0f - 2.0f * q.y * q.y - 2.0f * q.z * q.z;
	matrix[0][1] = 2.0f * q.x * q.y + 2.0f * q.w * q.z;
	matrix[0][2] = 2.0f * q.x * q.z - 2.0f * q.w * q.y;
	matrix[0][3] = 0.0f;
	matrix[1][0] = 2.0f * q.x * q.y - 2.0f * q.w * q.z;
	matrix[1][1] = 1.0f - 2.0f * q.x * q.x - 2.0f * q.z * q.z;
	matrix[1][2] = 2.0f * q.y * q.z + 2.0f * q.w * q.x;
	matrix[1][3] = 0.0f;
	matrix[2][0] = 2.0f * q.x * q.z + 2.0f * q.w * q.y;
	matrix[2][1] = 2.0f * q.y * q.z - 2.0f * q.w * q.x;
	matrix[2][2] = 1.0f - 2.0f * q.x * q.x - 2.0f * q.y * q.y;
	matrix[2][3] = 0.0f;
	matrix[3][0] = 0.0f;
	matrix[3][1] = 0.0f;
	matrix[3][2] = 0.0f;
	matrix[3][3] = 1.0f;
	return Matrix4x4(matrix);
}