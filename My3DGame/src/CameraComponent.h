//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include "Component.h"
#include "Math/Math.h"

class CameraComponent: public Component
{
public:
	CameraComponent(class Actor* owner, int updateOrder = 200);
	TypeID getType() const override
	{
		return TCameraComponent;
	}
protected:
	virtual void setViewMatrix(const Matrix4x4& view);
};