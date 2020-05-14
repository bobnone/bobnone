//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "CircleComponent.h"
#include "Actor.h"

CircleComponent::CircleComponent(class Actor* owner):Component(owner), radius_(0.0f)
{
	//EMPTY:
}
const Vector3& CircleComponent::getCenter() const
{
	return owner_->position();
}
float CircleComponent::getRadius() const
{
	return owner_->scale() * radius_;
}
bool intersect(const CircleComponent& a, const CircleComponent& b)
{
	//Calculate distance squared
	Vector3 diff = a.getCenter() - b.getCenter();
	float distSq = diff.length2();
	//Calculate sum of radii squared
	float radiiSq = a.getRadius() + b.getRadius();
	radiiSq *= radiiSq;
	return distSq <= radiiSq;
}