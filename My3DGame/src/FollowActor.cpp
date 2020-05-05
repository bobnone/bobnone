#include "FollowActor.h"
#include<GLM/glm.hpp>
#include "Mesh.h"
#include "Shader.h"
#include "Renderer.h"
#include "Skeleton.h"
#include "Animation.h"
#include "AudioSystem.h"
#include "JsonHelper.h"

FollowActor::FollowActor(Game* game):Actor(game), mAnimated(false)
{
	mMeshComp = new SkeletalMeshComponent(this);
	Renderer* renderer = mGame->GetRenderer();
	Mesh* mesh = renderer->GetMesh("Assets/CatWarrior.gpmesh");
	mMeshComp->SetMesh(mesh);
	mMeshComp->SetShader(renderer->GetShader("Skinned"));
	mMeshComp->SetSkeleton(mGame->GetSkeleton("Assets/CatWarrior.gpskel"));
	mMeshComp->PlayAnimation(mGame->GetAnimation("Assets/CatActionIdle.gpanim"));
	SetPosition(vector3(0.0f, -100.0f, 0.0f));
	mMoveComp = new MoveComponent(this);	
	mCameraComp = new FollowCamera(this);
	mCameraComp->SnapToIdeal();
	mAudioComp = new AudioComponent(this);
	mLastFootstep = 0.0f;
	mFootstep = mAudioComp->PlayEvent("event:/Footstep");
	mFootstep.SetPaused(true);
	// Add a component for the mirror camera
	MirrorCamera* mirror = new MirrorCamera(this);
	mirror->SnapToIdeal();
	mGame->SetFollowActor(this);
}
void FollowActor::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
	// Play the footstep if we're moving and haven't recently
	mLastFootstep -= deltaTime;
	if (!Math::NearZero(mMoveComp->GetForwardSpeed()) && mLastFootstep <= 0.0f)
	{
		mFootstep.SetPaused(false);
		mFootstep.Restart();
		mLastFootstep = 0.5f;
	}
}
void FollowActor::ActorInput(const uint8_t* keys)
{
	float angularXSpeed = 0.0f;
	float angularYSpeed = 0.0f;
	float angularZSpeed = 0.0f;
	float forwardSpeed = 0.0f;
	float strafeSpeed = 0.0f;
	float jumpSpeed = 0.0f;
	// wasd movement
	if (keys[SDL_SCANCODE_W])
	{
		forwardSpeed += 400.0f;
	}
	else if (keys[SDL_SCANCODE_S])
	{
		forwardSpeed -= 400.0f;
	}
	if (keys[SDL_SCANCODE_A])
	{
		strafeSpeed -= 400;
	}
	else if (keys[SDL_SCANCODE_D])
	{
		strafeSpeed += 400;
	}
	if (keys[SDL_SCANCODE_SPACE])
	{
		jumpSpeed += 400;
	}
	else if (keys[SDL_SCANCODE_Z])//idk why it doesn't like ctrl; modifier key?
	{
		jumpSpeed -= 400;
	}
	if (keys[SDL_SCANCODE_INSERT])
	{
		angularXSpeed -= Math::Pi;
	}
	else if (keys[SDL_SCANCODE_DELETE])
	{
		angularXSpeed += Math::Pi;
	}
	if (keys[SDL_SCANCODE_HOME])
	{
		angularYSpeed -= Math::Pi;
	}
	else if (keys[SDL_SCANCODE_END])
	{
		angularYSpeed += Math::Pi;
	}
	if (keys[SDL_SCANCODE_PAGEUP])
	{
		angularZSpeed -= Math::Pi;
	}
	else if (keys[SDL_SCANCODE_PAGEDOWN])
	{
		angularZSpeed += Math::Pi;
	}
	// Did we just start moving forward?
	if (!mAnimated && !Math::NearZero(forwardSpeed))
	{
		mAnimated = true;
		mMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/CatRunSprint.gpanim"), 1.25f);
	}
	// Or did we just stop moving forward?
	else if (mAnimated && Math::NearZero(forwardSpeed))
	{
		mAnimated = false;
		mMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/CatActionIdle.gpanim"));
	}
	mMoveComp->SetSpeed(angularXSpeed, angularYSpeed, angularZSpeed, forwardSpeed, strafeSpeed, jumpSpeed);
}
void FollowActor::SetVisible(bool visible)
{
	mMeshComp->SetVisible(visible);
}
void FollowActor::SetFootstepSurface(float value)
{
	// Pause here because the way I setup the parameter in FMOD
	// changing it will play a footstep
	mFootstep.SetPaused(true);
	mFootstep.SetParameter("Surface", value);
}