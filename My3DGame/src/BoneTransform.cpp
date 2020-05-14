//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "BoneTransform.h"

Matrix4x4 BoneTransform::toMatrix() const
{
	Matrix4x4 rot = Math::toMatrix(rotation_);
	Matrix4x4 trans = Math::createTranslationMatrix(translation_);
	return rot * trans;
}
BoneTransform BoneTransform::interpolate(const BoneTransform& a, const BoneTransform& b, float f)
{
	BoneTransform retVal;
	retVal.rotation_ = Math::slerp(a.rotation_, b.rotation_, f);
	retVal.translation_ = Math::lerp(a.translation_, b.translation_, f);
	return retVal;
}