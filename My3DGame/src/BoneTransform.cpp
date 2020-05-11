//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "BoneTransform.h"

matrix4 BoneTransform::toMatrix() const
{
	matrix4 rot = matrix4::CreateFromQuaternion(rotation_);
	matrix4 trans = matrix4::CreateTranslation(translation_);
	return rot * trans;
}
BoneTransform BoneTransform::interpolate(const BoneTransform& a, const BoneTransform& b, float f)
{
	BoneTransform retVal;
	retVal.rotation_ = quaternion::Slerp(a.rotation_, b.rotation_, f);
	retVal.translation_ = vector3::Lerp(a.translation_, b.translation_, f);
	return retVal;
}