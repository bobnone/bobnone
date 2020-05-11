//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "MirrorCamera.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "JsonHelper.h"

MirrorCamera::MirrorCamera(Actor* owner) :CameraComponent(owner), horzDist_(150.0f), vertDist_(200.0f), targetDist_(400.0f)
{
	//EMPTY:
}
void MirrorCamera::update(float deltaTime)
{
	CameraComponent::update(deltaTime);
	//Compute ideal position
	vector3 idealPos = computeCameraPos();
	//Target is target dist in front of owning actor
	vector3 target = owner_->position() - owner_->getForward() * targetDist_;
	//Use actual position here, not ideal
	matrix4 view = matrix4::CreateLookAt(idealPos, target, vector3::UnitZ);
	owner_->game()->renderer()->setMirrorView(view);
}
void MirrorCamera::snapToIdeal()
{
	vector3 idealPos = computeCameraPos();
	//Compute target and view
	vector3 target = owner_->position() - owner_->getForward() * targetDist_;
	//Use actual position here, not ideal
	matrix4 view = matrix4::CreateLookAt(idealPos, target, vector3::UnitZ);
	owner_->game()->renderer()->setMirrorView(view);
}
void MirrorCamera::loadProperties(const rapidjson::Value& inObj)
{
	CameraComponent::loadProperties(inObj);
	JsonHelper::getFloat(inObj, "horzDist", horzDist_);
	JsonHelper::getFloat(inObj, "vertDist", vertDist_);
	JsonHelper::getFloat(inObj, "targetDist", targetDist_);
}
void MirrorCamera::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const
{
	CameraComponent::saveProperties(alloc, inObj);
	JsonHelper::addFloat(alloc, inObj, "horzDist", horzDist_);
	JsonHelper::addFloat(alloc, inObj, "vertDist", vertDist_);
	JsonHelper::addFloat(alloc, inObj, "targetDist", targetDist_);
}
vector3 MirrorCamera::computeCameraPos() const
{
	//Set camera position in front of
	vector3 cameraPos = owner_->position();
	cameraPos += owner_->getForward() * horzDist_;
	cameraPos += vector3::UnitZ * vertDist_;
	return cameraPos;
}