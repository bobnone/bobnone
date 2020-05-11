//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include "MoveComponent.h"
#include <cstdint>

class InputComponent : public MoveComponent
{
public:
	//Lower update order to update first
	InputComponent(class Actor* owner);
	void processInput(const struct InputState& state) override;
	//Getters/setters for private variables
	float maxForwardSpeed() const
	{
		return maxForwardSpeed_;
	}
	float maxAngularSpeed() const
	{
		return maxAngularSpeed_;
	}
	int forwardKey() const
	{
		return forwardKey_;
	}
	int backKey() const
	{
		return backKey_;
	}
	int clockwiseKey() const
	{
		return clockwiseKey_;
	}
	int counterClockwiseKey() const
	{
		return counterClockwiseKey_;
	}
	void setMaxForwardSpeed(float speed)
	{
		maxForwardSpeed_ = speed;
	}
	void setMaxAngularSpeed(float speed)
	{
		maxAngularSpeed_ = speed;
	}
	void setForwardKey(int key)
	{
		forwardKey_ = key;
	}
	void setBackKey(int key)
	{
		backKey_ = key;
	}
	void setClockwiseKey(int key)
	{
		clockwiseKey_ = key;
	}
	void setCounterClockwiseKey(int key)
	{
		counterClockwiseKey_ = key;
	}
private:
	//The maximum forward/angular speeds
	float maxForwardSpeed_;
	float maxAngularSpeed_;
	//Keys for forward/back movement
	int forwardKey_;
	int backKey_;
	//Keys for angular movement
	int clockwiseKey_;
	int counterClockwiseKey_;
};