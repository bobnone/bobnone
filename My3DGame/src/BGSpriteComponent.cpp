//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "BGSpriteComponent.h"
#include "Actor.h"

BGSpriteComponent::BGSpriteComponent(class Actor* owner, int drawOrder):SpriteComponent(owner, drawOrder), scrollSpeed_(0.0f)
{
	//EMPTY:
}
void BGSpriteComponent::update(float deltaTime)
{
	SpriteComponent::update(deltaTime);
	for(auto& bg : bGTextures_)
	{
		//Update the x offset
		bg.offset_.x += scrollSpeed_ * deltaTime;
		//If this is completely off the screen, reset offset to
		//the right of the last bg texture
		if(bg.offset_.x < -screenSize_.x)
		{
			bg.offset_.x = (bGTextures_.size() - 1) * screenSize_.x - 1;
		}
	}
}
void BGSpriteComponent::draw(SDL_Renderer* renderer)
{
	//Draw each background texture
	for(auto& bg : bGTextures_)
	{
		SDL_Rect r;
		//Assume screen size dimensions
		r.w = static_cast<int>(screenSize_.x);
		r.h = static_cast<int>(screenSize_.y);
		//Center the rectangle around the position of the owner
		r.x = static_cast<int>(owner_->position().x - r.w / 2 + bg.offset_.x);
		r.y = static_cast<int>(owner_->position().y - r.h / 2 + bg.offset_.y);
		//Draw this background
		SDL_RenderCopy(renderer, bg.texture_, nullptr, &r);
	}
}
void BGSpriteComponent::setBGTextures(const std::vector<SDL_Texture*>& textures)
{
	int count = 0;
	for (auto tex : textures)
	{
		BGTexture temp;
		temp.texture_ = tex;
		//Each texture is screen width in offset
		temp.offset_.x = count * screenSize_.x;
		temp.offset_.y = 0;
		bGTextures_.emplace_back(temp);
		count++;
	}
}