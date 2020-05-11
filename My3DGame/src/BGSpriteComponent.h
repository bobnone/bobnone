//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include "SpriteComponent.h"
#include <vector>
#include "Math.h"

class BGSpriteComponent : public SpriteComponent
{
public:
	//Set draw order to default to lower (so it's in the background)
	BGSpriteComponent(class Actor* owner, int drawOrder = 10);
	//Update/draw overriden from parent
	void update(float deltaTime) override;
	void draw(SDL_Renderer* renderer) override;
	//Set the textures used for the background
	void setBGTextures(const std::vector<SDL_Texture*>& textures);
	//Get/set screen size and scroll speed
	void setScreenSize(const vector2& size)
	{
		screenSize_ = size;
	}
	void setScrollSpeed(float speed)
	{
		scrollSpeed_ = speed;
	}
	float scrollSpeed() const
	{
		return scrollSpeed_;
	}
private:
	// Struct to encapsulate each bg image and its offset
	struct BGTexture
	{
		SDL_Texture* texture_;
		vector2 offset_;
	};
	std::vector<BGTexture> bGTextures_;
	vector2 screenSize_;
	float scrollSpeed_;
};