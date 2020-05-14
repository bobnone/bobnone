//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include "Actor.h"

class Ship: public Actor
{
public:
	Ship(class Game* game);
	void updateActor(float deltaTime) override;
	void actorInput(const struct InputState& state) override;
private:
	Vector2 velocityDir_;
	Vector2 rotationDir_;
	float speed_;
	float laserCooldown_;
};