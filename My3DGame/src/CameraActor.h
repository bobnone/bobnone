//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include "Actor.h"
#include "SoundEvent.h"

class CameraActor : public Actor
{
public:
	CameraActor(class Game* game);
	void updateActor(float deltaTime) override;
	void actorInput(const uint8_t* keys) override;
	void setFootstepSurface(float value);
	const Vector3& getCameraPosition() const
	{
		return cameraPos_;
	}
private:
	class MoveComponent* moveComp_;
	class AudioComponent* audioComp_;
	Vector3 cameraPos_;
	SoundEvent footstep_;
	float lastFootstep_;
};