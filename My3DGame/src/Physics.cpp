//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "Physics.h"
#include <algorithm>
#include "BoxComponent.h"

Physics::Physics(Game* game):game_(game)
{
	//EMPTY:
}
bool Physics::segmentCast(const LineSegment& l, CollisionInfo& outColl)
{
	bool collided = false;
	//Initialize closestT to infinity, so first
	//intersection will always update closestT
	float closestT = Math::INFINITE_FLOAT;
	Vector3 norm;
	//Test against all boxes
	for(auto box: boxes_)
	{
		float t;
		//Does the segment intersect with the box?
		if(intersect(l, box->worldBox(), t, norm))
		{
			//Is this closer than previous intersection?
			if(t < closestT)
			{
				outColl.point_ = l.pointOnSegment(t);
				outColl.normal_ = norm;
				outColl.box_ = box;
				outColl.actor_ = box->owner();
				collided = true;
			}
		}
	}
	return collided;
}
void Physics::testPairwise(std::function<void(Actor*, Actor*)> f)
{
	//Naive implementation O(n^2)
	for(size_t i = 0; i < boxes_.size(); i++)
	{
		//Don't need to test vs itself and any previous i values
		for(size_t j = i + 1; j < boxes_.size(); j++)
		{
			BoxComponent* a = boxes_[i];
			BoxComponent* b = boxes_[j];
			if(intersect(a->worldBox(), b->worldBox()))
			{
				//Call supplied function to handle intersection
				f(a->owner(), b->owner());
			}
		}
	}
}
void Physics::testSweepAndPrune(std::function<void(Actor*, Actor*)> f)
{
	//Sort by min.x
	std::sort(boxes_.begin(), boxes_.end(), [](BoxComponent* a, BoxComponent* b) {
		return a->worldBox().min_.x < b->worldBox().min_.x;
	});
	for(size_t i = 0; i < boxes_.size(); i++)
	{
		//Get max.x for current box
		BoxComponent* a = boxes_[i];
		float max = a->worldBox().max_.x;
		for(size_t j = i + 1; j < boxes_.size(); j++)
		{
			BoxComponent* b = boxes_[j];
			//If AABB[j] min is past the max bounds of AABB[i],
			//then there aren't any other possible intersections
			//against AABB[i]
			if(b->worldBox().min_.x > max)
			{
				break;
			}
			else if(intersect(a->worldBox(), b->worldBox()))
			{
				f(a->owner(), b->owner());
			}
		}
	}
}
void Physics::addBox(BoxComponent* box)
{
	boxes_.emplace_back(box);
}
void Physics::removeBox(BoxComponent* box)
{
	auto i = std::find(boxes_.begin(), boxes_.end(), box);
	if(i != boxes_.end())
	{
		//Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(i, boxes_.end() - 1);
		boxes_.pop_back();
	}
}