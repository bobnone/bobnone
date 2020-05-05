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
	void UpdateActor(float deltaTime) override;
	void ActorInput(const uint8_t* keys) override;
	void SetVisible(bool visible);
	void SetFootstepSurface(float value);
	TypeID GetType() const override
	{
		return TFollowActor;
	}
private:
	class MoveComponent* mMoveComp;
	class AudioComponent* mAudioComp;
	SoundEvent mFootstep;
	float mLastFootstep;
	class FollowCamera* mCameraComp;
	class SkeletalMeshComponent* mMeshComp;
	bool mAnimated;
};