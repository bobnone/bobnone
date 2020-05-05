// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "MirrorCamera.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "JsonHelper.h"

MirrorCamera::MirrorCamera(Actor* owner) :CameraComponent(owner), mHorzDist(150.0f), mVertDist(200.0f), mTargetDist(400.0f)
{
	//
}
void MirrorCamera::Update(float deltaTime)
{
	CameraComponent::Update(deltaTime);
	// Compute ideal position
	vector3 idealPos = ComputeCameraPos();
	// Target is target dist in front of owning actor
	vector3 target = mOwner->GetPosition() - mOwner->GetForward() * mTargetDist;
	// Use actual position here, not ideal
	matrix4 view = matrix4::CreateLookAt(idealPos, target, vector3::UnitZ);
	Game* game = mOwner->GetGame();
	game->GetRenderer()->SetMirrorView(view);
}
void MirrorCamera::SnapToIdeal()
{
	vector3 idealPos = ComputeCameraPos();
	// Compute target and view
	vector3 target = mOwner->GetPosition() - mOwner->GetForward() * mTargetDist;
	// Use actual position here, not ideal
	matrix4 view = matrix4::CreateLookAt(idealPos, target, vector3::UnitZ);
	Game* game = mOwner->GetGame();
	game->GetRenderer()->SetMirrorView(view);
}
void MirrorCamera::LoadProperties(const rapidjson::Value& inObj)
{
	CameraComponent::LoadProperties(inObj);
	JsonHelper::GetFloat(inObj, "horzDist", mHorzDist);
	JsonHelper::GetFloat(inObj, "vertDist", mVertDist);
	JsonHelper::GetFloat(inObj, "targetDist", mTargetDist);
}
void MirrorCamera::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const
{
	CameraComponent::SaveProperties(alloc, inObj);
	JsonHelper::AddFloat(alloc, inObj, "horzDist", mHorzDist);
	JsonHelper::AddFloat(alloc, inObj, "vertDist", mVertDist);
	JsonHelper::AddFloat(alloc, inObj, "targetDist", mTargetDist);
}
vector3 MirrorCamera::ComputeCameraPos() const
{
	// Set camera position in front of
	vector3 cameraPos = mOwner->GetPosition();
	cameraPos += mOwner->GetForward() * mHorzDist;
	cameraPos += vector3::UnitZ * mVertDist;
	return cameraPos;
}