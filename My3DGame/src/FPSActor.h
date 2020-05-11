//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include "Actor.h"
#include "SoundEvent.h"

class FPSActor: public Actor
{
public:
	FPSActor(class Game* game);
	void updateActor(float deltaTime) override;
	void actorInput(const uint8_t* keys) override;
	void shoot();
	void setFootstepSurface(float value);
	void setVisible(bool visible);
	void fixCollisions();
private:
	class MoveComponent* moveComp_;
	class AudioComponent* audioComp_;
	class MeshComponent* meshComp_;
	class FPSCamera* cameraComp_;
	class BoxComponent* boxComp_;
	class Actor* fPSModel_;
	SoundEvent footstep_;
	float lastFootstep_;
};
