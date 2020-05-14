//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include "CameraComponent.h"
#include "Actor.h"

class FollowCamera: public CameraComponent
{
public:
	FollowCamera(class Actor* owner);
	void update(float deltaTime) override;
	void snapToIdeal();
	void setHorzDist(float dist)
	{
		horzDist_ = dist;
	}
	void setVertDist(float dist)
	{
		vertDist_ = dist;
	}
	void setTargetDist(float dist)
	{
		targetDist_ = dist;
	}
	void setSpringConstant(float spring)
	{
		springConstant_ = spring;
	}
	TypeID getType() const override
	{
		return TFollowCamera;
	}
	void loadProperties(const rapidjson::Value& inObj) override;
	void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const override;
private:
	void setViewMatrix(const Matrix4& view) override;
	Vector3 getVirtualPosition();
	Vector3 computeCameraPos() const;
	//Actual position of camera
	Vector3 actualPos_;
	//Velocity of actual camera
	Vector3 velocity_;
	//Horizontal follow distance
	float horzDist_;
	//Vertical follow distance
	float vertDist_;
	//Target distance
	float targetDist_;
	//Spring constant (higher is more stiff)
	float springConstant_;
};