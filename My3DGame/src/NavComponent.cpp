//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "NavComponent.h"
#include "Tile.h"

NavComponent::NavComponent(class Actor* owner, int updateOrder):MoveComponent(owner, updateOrder), nextNode_(nullptr)
{
	//EMPTY:
}
void NavComponent::update(float deltaTime)
{
	if(nextNode_)
	{
		//If we're at the next node, advance along path
		vector2 diff;
		diff.x = owner_->position().x - nextNode_->position().x;
		diff.y = owner_->position().y - nextNode_->position().y;
		if (Math::NearZero(diff.Length(), 2.0f))
		{
			nextNode_ = nextNode_->parent();
			turnTo(nextNode_->position());
		}
	}
	MoveComponent::update(deltaTime);
}
void NavComponent::startPath(const Tile* start)
{
	nextNode_ = start->parent();
	turnTo(nextNode_->position());
}
void NavComponent::turnTo(const vector2& pos)
{
	//Vector from me to pos
	vector2 dir = pos - owner_->position();
	//New angle is just atan2 of this dir vector
	//(Negate y because +y is down on screen)
	owner_->setRotation(Math::Atan2(-dir.y, dir.x));
}