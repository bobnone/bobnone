//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include "Math.h"

class BoneTransform
{
public:
	//For now, just make this data public
	quaternion rotation_;
	vector3 translation_;
	matrix4 toMatrix() const;
	static BoneTransform interpolate(const BoneTransform& a, const BoneTransform& b, float f);
};