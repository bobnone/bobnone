//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include "Actor.h"

class SplineActor : public Actor
{
public:
	SplineActor(class Game* game);
	void actorInput(const uint8_t* keys) override;
	void restartSpline();
private:
	class SplineCamera* cameraComp_;
};