//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "BoxComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Physics.h"
#include "JsonHelper.h"

BoxComponent::BoxComponent(Actor* owner, int updateOrder):Component(owner, updateOrder), objectBox_(Vector3(), Vector3()), worldBox_(Vector3(), Vector3()), shouldRotate_(true)
{
	owner->game()->physics()->addBox(this);
}
BoxComponent::~BoxComponent()
{
	owner_->game()->physics()->removeBox(this);
}
void BoxComponent::onUpdateWorldTransform()
{
	//Reset to object space box
	worldBox_ = objectBox_;
	//Scale
	worldBox_.min_ *= owner_->scale();
	worldBox_.max_ *= owner_->scale();
	//Rotate (if we want to)
	if(shouldRotate_)
	{
		worldBox_.rotate(owner_->rotation());
	}
	// Translate
	worldBox_.min_ += owner_->position();
	worldBox_.max_ += owner_->position();
}
void BoxComponent::loadProperties(const rapidjson::Value& inObj)
{
	Component::loadProperties(inObj);
	JsonHelper::getVector3(inObj, "objectMin", objectBox_.min_);
	JsonHelper::getVector3(inObj, "objectMax", objectBox_.max_);
	JsonHelper::getVector3(inObj, "worldMin", worldBox_.min_);
	JsonHelper::getVector3(inObj, "worldMax", worldBox_.max_);
	JsonHelper::getBool(inObj, "shouldRotate", shouldRotate_);
}
void BoxComponent::saveProperties(rapidjson::Document::AllocatorType & alloc, rapidjson::Value & inObj) const
{
	Component::saveProperties(alloc, inObj);
	JsonHelper::addVector3(alloc, inObj, "objectMin", objectBox_.min_);
	JsonHelper::addVector3(alloc, inObj, "objectMax", objectBox_.max_);
	JsonHelper::addVector3(alloc, inObj, "worldMin", worldBox_.min_);
	JsonHelper::addVector3(alloc, inObj, "worldMax", worldBox_.max_);
	JsonHelper::addBool(alloc, inObj, "shouldRotate", shouldRotate_);
}