// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "FollowCamera.h"
#include "JsonHelper.h"
#include "AudioSystem.h"
#include "Renderer.h"
#include "Game.h"

FollowCamera::FollowCamera(Actor* owner) :CameraComponent(owner), mHorzDist(350.0f), mVertDist(250.0f), mTargetDist(100.0f), mSpringConstant(128.0f)
{
	//
}
void FollowCamera::Update(float deltaTime)
{
	CameraComponent::Update(deltaTime);
	// Compute dampening from spring constant
	float dampening = 2.0f * Math::Sqrt(mSpringConstant);
	// Compute ideal position
	vector3 idealPos = ComputeCameraPos();
	// Compute difference between actual and ideal
	vector3 diff = mActualPos - idealPos;
	// Compute acceleration of spring
	vector3 acel = -mSpringConstant * diff - dampening * mVelocity;
	// Update velocity
	mVelocity += acel * deltaTime;
	// Update actual camera position
	mActualPos += mVelocity * deltaTime;
	// Target is target dist in front of owning actor
	vector3 target = mOwner->GetPosition() + mOwner->GetForward() * mTargetDist;
	// Use actual position here, not ideal
	matrix4 view = matrix4::CreateLookAt(mActualPos, target, vector3::UnitZ);
	SetViewMatrix(view);
}
void FollowCamera::SnapToIdeal()
{
	// Set actual position to ideal
	mActualPos = ComputeCameraPos();
	// Zero velocity
	mVelocity = vector3::Zero;
	// Compute target and view
	vector3 target = mOwner->GetPosition() + mOwner->GetForward() * mTargetDist;
	// Use actual position here, not ideal
	matrix4 view = matrix4::CreateLookAt(mActualPos, target, vector3::UnitZ);
	SetViewMatrix(view);
}
void FollowCamera::LoadProperties(const rapidjson::Value& inObj)
{
	CameraComponent::LoadProperties(inObj);
	JsonHelper::GetVector3(inObj, "actualPos", mActualPos);
	JsonHelper::GetVector3(inObj, "velocity", mVelocity);
	JsonHelper::GetFloat(inObj, "horzDist", mHorzDist);
	JsonHelper::GetFloat(inObj, "vertDist", mVertDist);
	JsonHelper::GetFloat(inObj, "targetDist", mTargetDist);
	JsonHelper::GetFloat(inObj, "springConstant", mSpringConstant);
}
void FollowCamera::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const
{
	CameraComponent::SaveProperties(alloc, inObj);
	JsonHelper::AddVector3(alloc, inObj, "actualPos", mActualPos);
	JsonHelper::AddVector3(alloc, inObj, "velocity", mVelocity);
	JsonHelper::AddFloat(alloc, inObj, "horzDist", mHorzDist);
	JsonHelper::AddFloat(alloc, inObj, "vertDist", mVertDist);
	JsonHelper::AddFloat(alloc, inObj, "targetDist", mTargetDist);
	JsonHelper::AddFloat(alloc, inObj, "springConstant", mSpringConstant);
}
void FollowCamera::SetViewMatrix(const matrix4& view)
{
	// Pass view matrix to renderer and audio system
	mOwner->GetGame()->GetRenderer()->SetViewMatrix(view);
	mOwner->GetGame()->GetAudioSystem()->SetListener(mOwner, view, GetVirtualPosition());
}
vector3 FollowCamera::GetVirtualPosition()
{
	vector3 playerPos = vector3::Normalize(mOwner->GetPosition());
	vector3 cameraPos = vector3::Normalize(mActualPos);
	vector3 virtualPos = playerPos * (mActualPos / cameraPos);
	return virtualPos;
}
vector3 FollowCamera::ComputeCameraPos() const
{
	// Set camera position behind and above owner
	vector3 cameraPos = mOwner->GetPosition();
	cameraPos -= mOwner->GetForward() * mHorzDist;
	cameraPos += vector3::UnitZ * mVertDist;
	return cameraPos;
}