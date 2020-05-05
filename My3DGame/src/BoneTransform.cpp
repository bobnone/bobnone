// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "BoneTransform.h"

matrix4 BoneTransform::ToMatrix() const
{
	matrix4 rot = matrix4::CreateFromQuaternion(mRotation);
	matrix4 trans = matrix4::CreateTranslation(mTranslation);
	return rot * trans;
}
BoneTransform BoneTransform::Interpolate(const BoneTransform& a, const BoneTransform& b, float f)
{
	BoneTransform retVal;
	retVal.mRotation = quaternion::Slerp(a.mRotation, b.mRotation, f);
	retVal.mTranslation = vector3::Lerp(a.mTranslation, b.mTranslation, f);
	return retVal;
}