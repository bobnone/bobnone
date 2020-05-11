//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include "MoveComponent.h"

class BallMove: public MoveComponent
{
public:
	BallMove(class Actor* owner);
	void update(float deltaTime) override;
	TypeID getType() const override
	{
		return TBallMove;
	}
protected:
};