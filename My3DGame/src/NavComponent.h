//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include "MoveComponent.h"
#include "Math/Math.h"

class NavComponent: public MoveComponent
{
public:
	//Lower update order to update first
	NavComponent(class Actor* owner, int updateOrder = 10);
	void update(float deltaTime) override;
	void startPath(const class Tile* start);
	void turnTo(const Vector2& pos);
private:
	const class Tile* nextNode_;
};