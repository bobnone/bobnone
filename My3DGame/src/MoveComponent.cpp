#include "MoveComponent.h"
#include "Actor.h"
#include "JsonHelper.h"

MoveComponent::MoveComponent(class Actor* owner, int updateOrder):Component(owner, updateOrder), angularXSpeed_(0.0f), angularYSpeed_(0.0f), angularZSpeed_(0.0f), forwardSpeed_(0.0f), strafeSpeed_(0.0f), jumpSpeed_(0.0f)
{
	//EMPTY:
}
void MoveComponent::update(float deltaTime)
{
	if(!Math::nearZero(angularXSpeed_))
	{
		Quaternion rot = owner_->rotation();
		float angle = angularXSpeed_ * deltaTime;
		//Create quaternion for incremental rotation
		//(Rotate about up axis)
		Quaternion inc(Vector3::UNIT_X, angle);
		//Concatenate old and new quaternion
		rot = Math::concatenate(rot, inc);
		owner_->setRotation(rot);
	}
	if(!Math::nearZero(angularYSpeed_))
	{
		Quaternion rot = owner_->rotation();
		float angle = angularYSpeed_ * deltaTime;
		//Create quaternion for incremental rotation
		//(Rotate about up axis)
		Quaternion inc(Vector3::UNIT_Y, angle);
		//Concatenate old and new quaternion
		rot = Math::concatenate(rot, inc);
		owner_->setRotation(rot);
	}
	if(!Math::nearZero(angularZSpeed_))
	{
		Quaternion rot = owner_->rotation();
		float angle = angularZSpeed_ * deltaTime;
		//Create quaternion for incremental rotation
		//(Rotate about up axis)
		Quaternion inc(Vector3::UNIT_Z, angle);
		//Concatenate old and new quaternion
		rot = Math::concatenate(rot, inc);
		owner_->setRotation(rot);
	}
	if(!Math::nearZero(forwardSpeed_) || !Math::nearZero(strafeSpeed_) || !Math::nearZero(jumpSpeed_))
	{
		Vector3 pos = owner_->position();
		pos += owner_->getForward() * forwardSpeed_ * deltaTime;
		pos += owner_->getRight() * strafeSpeed_ * deltaTime;
		pos += owner_->getUp() * jumpSpeed_ * deltaTime;
		owner_->setPosition(pos);
	}
}
void MoveComponent::loadProperties(const rapidjson::Value& inObj)
{
	Component::loadProperties(inObj);
	JsonHelper::getFloat(inObj, "angularXSpeed", angularXSpeed_);
	JsonHelper::getFloat(inObj, "angularYSpeed", angularYSpeed_);
	JsonHelper::getFloat(inObj, "angularZSpeed", angularZSpeed_);
	JsonHelper::getFloat(inObj, "forwardSpeed", forwardSpeed_);
	JsonHelper::getFloat(inObj, "strafeSpeed", strafeSpeed_);
	JsonHelper::getFloat(inObj, "jumpSpeed", jumpSpeed_);
}
void MoveComponent::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const
{
	Component::saveProperties(alloc, inObj);
	JsonHelper::addFloat(alloc, inObj, "angularXSpeed", angularXSpeed_);
	JsonHelper::addFloat(alloc, inObj, "angularYSpeed", angularYSpeed_);
	JsonHelper::addFloat(alloc, inObj, "angularZSpeed", angularZSpeed_);
	JsonHelper::addFloat(alloc, inObj, "forwardSpeed", forwardSpeed_);
	JsonHelper::addFloat(alloc, inObj, "strafeSpeed", strafeSpeed_);
	JsonHelper::addFloat(alloc, inObj, "jumpSpeed", jumpSpeed_);
}