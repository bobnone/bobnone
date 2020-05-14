//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include "CameraComponent.h"

class OrbitCamera: public CameraComponent
{
public:
	OrbitCamera(class Actor* owner);
	void update(float deltaTime) override;
	float pitchSpeed() const
	{
		return pitchSpeed_;
	}
	float yawSpeed() const
	{
		return yawSpeed_;
	}
	void setPitchSpeed(float speed)
	{
		pitchSpeed_ = speed;
	}
	void setYawSpeed(float speed)
	{
		yawSpeed_ = speed;
	}
private:
	//Offset from target
	Vector3 offset_;
	//Up vector of camera
	Vector3 up_;
	//Rotation/sec speed of pitch
	float pitchSpeed_;
	//Rotation/sec speed of yaw
	float yawSpeed_;
};