// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "BallMove.h"
#include "Actor.h"
#include "Game.h"
#include "Physics.h"
#include "TargetActor.h"
#include "BallActor.h"

BallMove::BallMove(Actor* owner) :MoveComponent(owner)
{
	//EMPTY:
}
void BallMove::update(float deltaTime)
{
	const float segmentLength = 30.0f;
	Physics* physics = owner_->game()->physics();
	// Construct segment in direction of travel
	vector3 start = owner_->position();
	vector3 dir = owner_->getForward();
	vector3 end = start + dir * segmentLength;
	// Create line segment
	LineSegment l(start, end);
	// Test segment vs world
	Physics::CollisionInfo info;
	if(physics->segmentCast(l, info))
	{
		// If we collided, reflect the ball about the normal
		dir = vector3::Reflect(dir, info.normal_);
		owner_->rotateToNewForward(dir);
		// Did we hit a target?
		TargetActor* target = dynamic_cast<TargetActor*>(info.actor_);
		if (target)
		{
			static_cast<BallActor*>(owner_)->hitTarget();
		}
	}
	MoveComponent::update(deltaTime);
}