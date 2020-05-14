//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "SpriteComponent.h"
#include "Math.h"
#include "Texture.h"
#include "Shader.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "JsonHelper.h"

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder):Component(owner), texture_(nullptr), drawOrder_(drawOrder), texWidth_(0), texHeight_(0), visible_(true)
{
	owner_->game()->renderer()->addSprite(this);
}
SpriteComponent::~SpriteComponent()
{
	owner_->game()->renderer()->removeSprite(this);
}
void SpriteComponent::draw(Shader* shader)
{
	if(texture_)
	{
		//Scale the quad by the width/height of texture
		Matrix4x4 scaleMat = Math::createScaleMatrix(static_cast<float>(texWidth_), static_cast<float>(texHeight_), 1.0f);
		Matrix4x4 world = scaleMat * owner_->worldTransform();
		//Since all sprites use the same shader/vertices,
		//the game first sets them active before any sprite draws
		//Set world transform
		shader->setMatrixUniform("uWorldTransform", world);
		//Set current texture
		texture_->setActive();
		//Draw quad
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}
}
void SpriteComponent::setTexture(Texture* texture)
{
	texture_ = texture;
	//Set width/height
	texWidth_ = texture->getWidth();
	texHeight_ = texture->getHeight();
}
void SpriteComponent::loadProperties(const rapidjson::Value& inObj)
{
	Component::loadProperties(inObj);
	std::string texFile;
	if(JsonHelper::getString(inObj, "textureFile", texFile))
	{
		setTexture(owner_->game()->renderer()->getTexture(texFile));
	}
	JsonHelper::getInt(inObj, "drawOrder", drawOrder_);
	JsonHelper::getBool(inObj, "visible", visible_);
}
void SpriteComponent::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const
{
	Component::saveProperties(alloc, inObj);
	if(texture_)
	{
		JsonHelper::addString(alloc, inObj, "textureFile", texture_->fileName());
	}
	JsonHelper::addInt(alloc, inObj, "drawOrder", drawOrder_);
	JsonHelper::addBool(alloc, inObj, "visible", visible_);
}