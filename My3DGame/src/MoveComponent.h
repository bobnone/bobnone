#pragma once
#include "Component.h"

class MoveComponent: public Component
{
public:
	//Lower update order to update first
	MoveComponent(class Actor* owner, int updateOrder = 10);
	void update(float deltaTime) override;
	float angularXSpeed() const
	{
		return angularXSpeed_;
	}
	float angularYSpeed() const
	{
		return angularYSpeed_;
	}
	float angularZSpeed() const
	{
		return angularZSpeed_;
	}
	float forwardSpeed() const
	{
		return forwardSpeed_;
	}
	float strafeSpeed() const
	{
		return strafeSpeed_;
	}
	float jumpSpeed() const
	{
		return jumpSpeed_;
	}
	void setSpeed(float angularXSpeed, float angularYSpeed, float angularZSpeed, float forwardSpeed, float strafeSpeed, float jumpSpeed)
	{
		angularXSpeed_ = angularXSpeed;
		angularYSpeed_ = angularYSpeed;
		angularZSpeed_ = angularZSpeed;
		forwardSpeed_ = forwardSpeed;
		strafeSpeed_ = strafeSpeed;
		jumpSpeed_ = jumpSpeed;
	}
	void setAngularXSpeed(float speed)
	{
		angularXSpeed_ = speed;
	}
	void setAngularYSpeed(float speed)
	{
		angularYSpeed_ = speed;
	}
	void setAngularZSpeed(float speed)
	{
		angularZSpeed_ = speed;
	}
	void setForwardSpeed(float speed)
	{
		forwardSpeed_ = speed;
	}
	void setStrafeSpeed(float speed)
	{
		strafeSpeed_ = speed;
	}
	void setJumpSpeed(float speed)
	{
		strafeSpeed_ = speed;
	}
	TypeID getType() const override
	{
		return TMoveComponent;
	}
	void loadProperties(const rapidjson::Value& inObj) override;
	void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const override;
protected:
	float angularXSpeed_;
	float angularYSpeed_;
	float angularZSpeed_;
	float forwardSpeed_;
	float strafeSpeed_;
	float jumpSpeed_;
};