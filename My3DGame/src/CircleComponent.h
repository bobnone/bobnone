//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include "Component.h"
#include "Math/Math.h"

class CircleComponent: public Component
{
public:
	CircleComponent(class Actor* owner);
	void setRadius(float radius)
	{
		radius_ = radius;
	}
	float getRadius() const;
	const Vector3& getCenter() const;
private:
	float radius_;
};

bool intersect(const CircleComponent& a, const CircleComponent& b);