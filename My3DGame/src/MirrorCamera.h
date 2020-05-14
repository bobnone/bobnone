//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include "CameraComponent.h"

class MirrorCamera : public CameraComponent
{
public:
	MirrorCamera(class Actor* owner);
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
	TypeID getType() const override
	{
		return TMirrorCamera;
	}
	void loadProperties(const rapidjson::Value& inObj) override;
	void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const override;
private:
	Vector3 computeCameraPos() const;
	//Horizontal follow distance
	float horzDist_;
	//Vertical follow distance
	float vertDist_;
	//Target distance
	float targetDist_;
};