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
	float dampening = 2.0f * Math::sqrt(springConstant_);
	//Compute ideal position
	Vector3 idealPos = computeCameraPos();
	//Compute difference between actual and ideal
	Vector3 diff = actualPos_ - idealPos;
	//Compute acceleration of spring
	Vector3 acel = -springConstant_ * diff - dampening * velocity_;
	//Update velocity
	velocity_ += acel * deltaTime;
	//Update actual camera position
	actualPos_ += velocity_ * deltaTime;
	//Target is target dist in front of owning actor
	Vector3 target = owner_->position() + owner_->getForward() * targetDist_;
	//Use actual position here, not ideal
	Matrix4 view = Matrix4::createLookAt(actualPos_, target, Vector3::UNIT_Z);
	setViewMatrix(view);
}
void FollowCamera::snapToIdeal()
{
	//Set actual position to ideal
	actualPos_ = computeCameraPos();
	//Zero velocity
	velocity_ = Vector3();
	//Compute target and view
	Vector3 target = owner_->position() + owner_->getForward() * targetDist_;
	//Use actual position here, not ideal
	Matrix4 view = Matrix4::createLookAt(actualPos_, target, Vector3::UNIT_Z);
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
void FollowCamera::setViewMatrix(const Matrix4& view)
{
	//Pass view matrix to renderer and audio system
	owner_->game()->renderer()->setViewMatrix(view);
	owner_->game()->audioSystem()->setListener(owner_, view, getVirtualPosition());
}
Vector3 FollowCamera::getVirtualPosition()
{
	Vector3 playerPos = Vector3::normalize(owner_->position());
	Vector3 cameraPos = Vector3::normalize(actualPos_);
	Vector3 virtualPos = playerPos * (actualPos_ / cameraPos);
	return virtualPos;
}
Vector3 FollowCamera::computeCameraPos() const
{
	//Set camera position behind and above owner
	Vector3 cameraPos = owner_->position();
	cameraPos -= owner_->getForward() * horzDist_;
	cameraPos += Vector3::UNIT_Z * vertDist_;
	return cameraPos;
}