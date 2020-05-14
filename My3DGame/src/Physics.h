//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include <functional>
#include "Math.h"
#include "Collision.h"

class Physics
{
public:
	Physics(class Game* game);
	//Used to give helpful information about collision results
	struct CollisionInfo
	{
		//Point of collision
		Vector3 point_;
		//Normal at collision
		Vector3 normal_;
		//Component collided with
		class BoxComponent* box_ = nullptr;
		//Owning actor of component
		class Actor* actor_ = nullptr;
	};
	//Test a line segment against boxes
	//Returns true if it collides against a box
	bool segmentCast(const LineSegment& l, CollisionInfo& outColl);
	//Tests collisions using naive pairwise
	void testPairwise(std::function<void(class Actor*, class Actor*)> f);
	//Test collisions using sweep and prune
	void testSweepAndPrune(std::function<void(class Actor*, class Actor*)> f);
	//Add/remove box components from world
	void addBox(class BoxComponent* box);
	void removeBox(class BoxComponent* box);
private:
	class Game* game_;
	std::vector<class BoxComponent*> boxes_;
};