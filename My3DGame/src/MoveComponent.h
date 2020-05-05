#pragma once
#include "Component.h"

class MoveComponent : public Component
{
public:
	// Lower update order to update first
	MoveComponent(class Actor* owner, int updateOrder = 10);
	void Update(float deltaTime) override;
	float GetAngularXSpeed() const
	{
		return mAngularXSpeed;
	}
	float GetAngularYSpeed() const
	{
		return mAngularYSpeed;
	}
	float GetAngularZSpeed() const
	{
		return mAngularZSpeed;
	}
	float GetForwardSpeed() const
	{
		return mForwardSpeed;
	}
	float GetStrafeSpeed() const
	{
		return mStrafeSpeed;
	}
	float GetJumpSpeed() const
	{
		return mJumpSpeed;
	}
	void SetSpeed(float angularXSpeed, float angularYSpeed, float angularZSpeed, float forwardSpeed, float strafeSpeed, float jumpSpeed)
	{
		mAngularXSpeed = angularXSpeed;
		mAngularYSpeed = angularYSpeed;
		mAngularZSpeed = angularZSpeed;
		mForwardSpeed = forwardSpeed;
		mStrafeSpeed = strafeSpeed;
		mJumpSpeed = jumpSpeed;
	}
	void SetAngularXSpeed(float speed)
	{
		mAngularXSpeed = speed;
	}
	void SetAngularYSpeed(float speed)
	{
		mAngularYSpeed = speed;
	}
	void SetAngularZSpeed(float speed)
	{
		mAngularZSpeed = speed;
	}
	void SetForwardSpeed(float speed)
	{
		mForwardSpeed = speed;
	}
	void SetStrafeSpeed(float speed)
	{
		mStrafeSpeed = speed;
	}
	void SetJumpSpeed(float speed)
	{
		mStrafeSpeed = speed;
	}
	TypeID GetType() const override
	{
		return TMoveComponent;
	}
	void LoadProperties(const rapidjson::Value& inObj) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const override;
protected:
	float mAngularXSpeed;
	float mAngularYSpeed;
	float mAngularZSpeed;
	float mForwardSpeed;
	float mStrafeSpeed;
	float mJumpSpeed;
};