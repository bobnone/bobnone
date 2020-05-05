#include "MoveComponent.h"
#include "Actor.h"
#include "JsonHelper.h"

MoveComponent::MoveComponent(class Actor* owner, int updateOrder) :Component(owner, updateOrder), mAngularXSpeed(0.0f), mAngularYSpeed(0.0f), mAngularZSpeed(0.0f), mForwardSpeed(0.0f), mStrafeSpeed(0.0f), mJumpSpeed(0.0f)
{
	//
}
void MoveComponent::Update(float deltaTime)
{
	if (!Math::NearZero(mAngularXSpeed))
	{
		quaternion rot = mOwner->GetRotation();
		float angle = mAngularXSpeed * deltaTime;
		// Create quaternion for incremental rotation
		// (Rotate about up axis)
		quaternion inc(vector3::UnitX, angle);
		// Concatenate old and new quaternion
		rot = quaternion::Concatenate(rot, inc);
		mOwner->SetRotation(rot);
	}
	if (!Math::NearZero(mAngularYSpeed))
	{
		quaternion rot = mOwner->GetRotation();
		float angle = mAngularYSpeed * deltaTime;
		// Create quaternion for incremental rotation
		// (Rotate about up axis)
		quaternion inc(vector3::UnitY, angle);
		// Concatenate old and new quaternion
		rot = quaternion::Concatenate(rot, inc);
		mOwner->SetRotation(rot);
	}
	if (!Math::NearZero(mAngularZSpeed))
	{
		quaternion rot = mOwner->GetRotation();
		float angle = mAngularZSpeed * deltaTime;
		// Create quaternion for incremental rotation
		// (Rotate about up axis)
		quaternion inc(vector3::UnitZ, angle);
		// Concatenate old and new quaternion
		rot = quaternion::Concatenate(rot, inc);
		mOwner->SetRotation(rot);
	}
	if (!Math::NearZero(mForwardSpeed) || !Math::NearZero(mStrafeSpeed) || !Math::NearZero(mJumpSpeed))
	{
		vector3 pos = mOwner->GetPosition();
		pos += mOwner->GetForward() * mForwardSpeed * deltaTime;
		pos += mOwner->GetRight() * mStrafeSpeed * deltaTime;
		pos += mOwner->GetUp() * mJumpSpeed * deltaTime;
		mOwner->SetPosition(pos);
	}
}
void MoveComponent::LoadProperties(const rapidjson::Value& inObj)
{
	Component::LoadProperties(inObj);
	JsonHelper::GetFloat(inObj, "angularXSpeed", mAngularXSpeed);
	JsonHelper::GetFloat(inObj, "angularYSpeed", mAngularYSpeed);
	JsonHelper::GetFloat(inObj, "angularZSpeed", mAngularZSpeed);
	JsonHelper::GetFloat(inObj, "forwardSpeed", mForwardSpeed);
	JsonHelper::GetFloat(inObj, "strafeSpeed", mStrafeSpeed);
	JsonHelper::GetFloat(inObj, "jumpSpeed", mJumpSpeed);
}
void MoveComponent::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const
{
	Component::SaveProperties(alloc, inObj);
	JsonHelper::AddFloat(alloc, inObj, "angularXSpeed", mAngularXSpeed);
	JsonHelper::AddFloat(alloc, inObj, "angularYSpeed", mAngularYSpeed);
	JsonHelper::AddFloat(alloc, inObj, "angularZSpeed", mAngularZSpeed);
	JsonHelper::AddFloat(alloc, inObj, "forwardSpeed", mForwardSpeed);
	JsonHelper::AddFloat(alloc, inObj, "strafeSpeed", mStrafeSpeed);
	JsonHelper::AddFloat(alloc, inObj, "jumpSpeed", mJumpSpeed);
}