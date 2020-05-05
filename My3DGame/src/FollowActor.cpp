// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "FollowActor.h"
#include "SkeletalMeshComponent.h"
#include "Mesh.h"
#include "Shader.h"
#include "Game.h"
#include "Renderer.h"
#include "Skeleton.h"
#include "Animation.h"
#include "FollowCamera.h"
#include "MoveComponent.h"
#include "MirrorCamera.h"
#include "JsonHelper.h"

FollowActor::FollowActor(Game* game) :Actor(game), mMoving(false)
{
	mMeshComp = new SkeletalMeshComponent(this);
	Renderer* renderer = GetGame()->GetRenderer();
	Mesh* mesh = renderer->GetMesh("Assets/CatWarrior.gpmesh");
	mMeshComp->SetMesh(mesh);
	mMeshComp->SetShader(renderer->GetShader("Skinned"));
	mMeshComp->SetSkeleton(GetGame()->GetSkeleton("Assets/CatWarrior.gpskel"));
	mMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/CatActionIdle.gpanim"));
	SetPosition(vector3(0.0f, -100.0f, 0.0f));
	mMoveComp = new MoveComponent(this);
	mCameraComp = new FollowCamera(this);
	mCameraComp->SnapToIdeal();
	// Add a component for the mirror camera
	MirrorCamera* mirror = new MirrorCamera(this);
	mirror->SnapToIdeal();
	game->SetFollowActor(this);
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
	else if (keys[SDL_SCANCODE_Z])//idk why it dosent like ctrl; modifier key?
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
	if (!mMoving && !Math::NearZero(forwardSpeed))
	{
		mMoving = true;
		mMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/CatRunSprint.gpanim"), 1.25f);
	}
	// Or did we just stop moving forward?
	else if (mMoving && Math::NearZero(forwardSpeed))
	{
		mMoving = false;
		mMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/CatActionIdle.gpanim"));
	}
	mMoveComp->SetSpeed(angularXSpeed, angularYSpeed, angularZSpeed, forwardSpeed, strafeSpeed, jumpSpeed);
}
void FollowActor::SetVisible(bool visible)
{
	mMeshComp->SetVisible(visible);
}
void FollowActor::LoadProperties(const rapidjson::Value& inObj)
{
	Actor::LoadProperties(inObj);
	JsonHelper::GetBool(inObj, "moving", mMoving);
}
void FollowActor::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const
{
	Actor::SaveProperties(alloc, inObj);
	JsonHelper::AddBool(alloc, inObj, "moving", mMoving);
}