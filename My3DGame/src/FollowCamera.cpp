//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "FollowCamera.h"
#include "JsonHelper.h"
#include "AudioSystem.h"
#include "Renderer.h"
#include "Game.h"

FollowCamera::FollowCamera(Actor* owner):CameraComponent(owner), horzDist_(350.0f), vertDist_(250.0f), targetDist_(100.0f), springConstant_(128.0f)
{
	//EMPTY:
}
void FollowCamera::update(float deltaTime)
{
	CameraComponent::update(deltaTime);
	//Compute dampening from spring constant
	float dampening = 2.0f * Math::Sqrt(springConstant_);
	//Compute ideal position
	vector3 idealPos = computeCameraPos();
	//Compute difference between actual and ideal
	vector3 diff = actualPos_ - idealPos;
	//Compute acceleration of spring
	vector3 acel = -springConstant_ * diff - dampening * velocity_;
	//Update velocity
	velocity_ += acel * deltaTime;
	//Update actual camera position
	actualPos_ += velocity_ * deltaTime;
	//Target is target dist in front of owning actor
	vector3 target = owner_->position() + owner_->getForward() * targetDist_;
	//Use actual position here, not ideal
	matrix4 view = matrix4::CreateLookAt(actualPos_, target, vector3::UnitZ);
	setViewMatrix(view);
}
void FollowCamera::snapToIdeal()
{
	//Set actual position to ideal
	actualPos_ = computeCameraPos();
	//Zero velocity
	velocity_ = vector3::Zero;
	// Compute target and view
	vector3 target = owner_->position() + owner_->getForward() * targetDist_;
	// Use actual position here, not ideal
	matrix4 view = matrix4::CreateLookAt(actualPos_, target, vector3::UnitZ);
	setViewMatrix(view);
}
void FollowCamera::loadProperties(const rapidjson::Value& inObj)
{
	CameraComponent::loadProperties(inObj);
	JsonHelper::getVector3(inObj, "actualPos", actualPos_);
	JsonHelper::getVector3(inObj, "velocity", velocity_);
	JsonHelper::getFloat(inObj, "horzDist", horzDist_);
	JsonHelper::getFloat(inObj, "vertDist", vertDist_);
	JsonHelper::getFloat(inObj, "targetDist", targetDist_);
	JsonHelper::getFloat(inObj, "springConstant", springConstant_);
}
void FollowCamera::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const
{
	CameraComponent::saveProperties(alloc, inObj);
	JsonHelper::addVector3(alloc, inObj, "actualPos", actualPos_);
	JsonHelper::addVector3(alloc, inObj, "velocity", velocity_);
	JsonHelper::addFloat(alloc, inObj, "horzDist", horzDist_);
	JsonHelper::addFloat(alloc, inObj, "vertDist", vertDist_);
	JsonHelper::addFloat(alloc, inObj, "targetDist", targetDist_);
	JsonHelper::addFloat(alloc, inObj, "springConstant", springConstant_);
}
void FollowCamera::setViewMatrix(const matrix4& view)
{
	//Pass view matrix to renderer and audio system
	owner_->game()->renderer()->setViewMatrix(view);
	owner_->game()->audioSystem()->setListener(owner_, view, getVirtualPosition());
}
vector3 FollowCamera::getVirtualPosition()
{
	vector3 playerPos = vector3::Normalize(owner_->position());
	vector3 cameraPos = vector3::Normalize(actualPos_);
	vector3 virtualPos = playerPos * (actualPos_ / cameraPos);
	return virtualPos;
}
vector3 FollowCamera::computeCameraPos() const
{
	//Set camera position behind and above owner
	vector3 cameraPos = owner_->position();
	cameraPos -= owner_->getForward() * horzDist_;
	cameraPos += vector3::UnitZ * vertDist_;
	return cameraPos;
}