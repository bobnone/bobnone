//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include "Actor.h"

class Bullet: public Actor
{
public:
	Bullet(class Game* game);
	void updateActor(float deltaTime) override;
private:
	class CircleComponent* circle_;
	float liveTime_;
};