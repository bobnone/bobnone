#pragma once
#include "Actor.h"
#include "Game.h"
#include "SoundEvent.h"
#include "FollowCamera.h"
#include "MoveComponent.h"
#include "MirrorCamera.h"
#include "AudioComponent.h"
#include "SkeletalMeshComponent.h"

class FollowActor : public Actor
{
public:
	FollowActor(class Game* game);
	void updateActor(float deltaTime) override;
	void actorInput(const uint8_t* keys) override;
	void setVisible(bool visible);
	void setFootstepSurface(float value);
	TypeID getType() const override
	{
		return TFollowActor;
	}
private:
	class MoveComponent* moveComp_;
	class AudioComponent* audioComp_;
	SoundEvent footstep_;
	float lastFootstep_;
	class FollowCamera* cameraComp_;
	class SkeletalMeshComponent* meshComp_;
	bool animated_;
};