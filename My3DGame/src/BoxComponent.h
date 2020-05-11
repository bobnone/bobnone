//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include "Component.h"
#include "Collision.h"

class BoxComponent : public Component
{
public:
	BoxComponent(class Actor* owner, int updateOrder = 100);
	~BoxComponent();
	void onUpdateWorldTransform() override;
	void setObjectBox(const AABB& model)
	{
		objectBox_ = model;
	}
	const AABB& worldBox() const
	{
		return worldBox_;
	}
	TypeID getType() const override
	{
		return TBoxComponent;
	}
	void loadProperties(const rapidjson::Value& inObj) override;
	void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const override;
	void setShouldRotate(bool value)
	{
		shouldRotate_ = value;
	}
private:
	AABB objectBox_;
	AABB worldBox_;
	bool shouldRotate_;
};