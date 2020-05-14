//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "BoneTransform.h"

Matrix4 BoneTransform::toMatrix() const
{
	Matrix4 rot = Matrix4::fromQuaternion(rotation_);
	Matrix4 trans = Matrix4::createTranslation(translation_);
	return rot * trans;
}
BoneTransform BoneTransform::interpolate(const BoneTransform& a, const BoneTransform& b, float f)
{
	BoneTransform retVal;
	retVal.rotation_ =Quaternion::slerp(a.rotation_, b.rotation_, f);
	retVal.translation_ = Vector3::lerp(a.translation_, b.translation_, f);
	return retVal;
}