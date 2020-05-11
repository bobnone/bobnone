//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "Component.h"
#include "Actor.h"
#include "JsonHelper.h"

const char* Component::TypeNames[NUM_COMPONENT_TYPES] = {
	"Component",
	"AudioComponent",
	"BoxComponent",
	"CameraComponent",
	"FollowCamera",
	"MeshComponent",
	"MoveComponent",
	"SkeletalMeshComponent",
	"SpriteComponent",
	"MirrorCamera",
	"PointLightComponent",
	"TargetComponent"
};
Component::Component(Actor* owner, int updateOrder): owner_(owner), updateOrder_(updateOrder)
{
	//Add to actor's vector of components
	owner_->addComponent(this);
}
Component::~Component()
{
	owner_->removeComponent(this);
}
void Component::update(float deltaTime)
{
	//EMPTY:
}
void Component::onUpdateWorldTransform()
{
	//EMPTY:
}
void Component::loadProperties(const rapidjson::Value& inObj)
{
	JsonHelper::getInt(inObj, "updateOrder", updateOrder_);
}
void Component::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const
{
	JsonHelper::addInt(alloc, inObj, "updateOrder", updateOrder_);
}