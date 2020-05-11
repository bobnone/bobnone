//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include "Component.h"

class SpriteComponent : public Component
{
public:
	//(Lower draw order corresponds with further back)
	SpriteComponent(class Actor* owner, int drawOrder = 100);
	~SpriteComponent();
	virtual void draw(class Shader* shader);
	virtual void setTexture(class Texture* texture);
	int drawOrder() const
	{
		return drawOrder_;
	}
	int texHeight() const
	{
		return texHeight_;
	}
	int texWidth() const
	{
		return texWidth_;
	}
	void setVisible(bool visible)
	{
		visible_ = visible;
	}
	bool visible() const
	{
		return visible_;
	}
	TypeID getType() const override
	{
		return TSpriteComponent;
	}
	void loadProperties(const rapidjson::Value& inObj) override;
	void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const override;
protected:
	class Texture* texture_;
	int drawOrder_;
	int texWidth_;
	int texHeight_;
	bool visible_;
};