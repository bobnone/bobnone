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
	Vector3 idealPos = computeCameraPos();
	//Target is target dist in front of owning actor
	Vector3 target = owner_->position() - owner_->getForward() * targetDist_;
	//Use actual position here, not ideal
	Matrix4 view = Matrix4::createLookAt(idealPos, target, Vector3::UNIT_Z);
	owner_->game()->renderer()->setMirrorView(view);
}
void MirrorCamera::snapToIdeal()
{
	Vector3 idealPos = computeCameraPos();
	//Compute target and view
	Vector3 target = owner_->position() - owner_->getForward() * targetDist_;
	//Use actual position here, not ideal
	Matrix4 view = Matrix4::createLookAt(idealPos, target, Vector3::UNIT_Z);
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
Vector3 MirrorCamera::computeCameraPos() const
{
	//Set camera position in front of
	Vector3 cameraPos = owner_->position();
	cameraPos += owner_->getForward() * horzDist_;
	cameraPos += Vector3::UNIT_Z * vertDist_;
	return cameraPos;
}