//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include "Actor.h"

class OrbitActor : public Actor
{
public:
	OrbitActor(class Game* game);
	void actorInput(const uint8_t* keys) override;
	void setVisible(bool visible);
private:
	class OrbitCamera* cameraComp_;
	class MeshComponent* meshComp_;
};