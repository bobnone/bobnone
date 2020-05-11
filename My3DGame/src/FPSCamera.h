//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include "CameraComponent.h"

class FPSCamera : public CameraComponent
{
public:
	FPSCamera(class Actor* owner);
	void update(float deltaTime) override;
	float pitch() const
	{
		return pitch_;
	}
	float pitchSpeed() const
	{
		return pitchSpeed_;
	}
	float maxPitch() const
	{
		return maxPitch_;
	}
	void setPitchSpeed(float speed)
	{
		pitchSpeed_ = speed;
	}
	void setMaxPitch(float pitch)
	{
		maxPitch_ = pitch;
	}
private:
	//Rotation/sec speed of pitch
	float pitchSpeed_;
	//Maximum pitch deviation from forward
	float maxPitch_;
	//Current pitch
	float pitch_;
};