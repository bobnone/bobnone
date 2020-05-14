//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "AnimatedSpriteComponent.h"

AnimatedSpriteComponent::AnimatedSpriteComponent(Actor* owner, int drawOrder):SpriteComponent(owner, drawOrder), currentFrame_(0.0f), FPS_(24.0f)
{
	//EMPTY:
}
void AnimatedSpriteComponent::update(float deltaTime)
{
	SpriteComponent::update(deltaTime);
	if(textures_.size() > 0)
	{
		//Update the current frame based on frame rate
		//and delta time
		currentFrame_ += FPS_ * deltaTime;
		//Wrap current frame if needed
		while(currentFrame_ >= textures_.size())
		{
			currentFrame_ -= textures_.size();
		}
		//Set the current texture
		setTexture(textures_[static_cast<int>(currentFrame_)]);
	}
}
void AnimatedSpriteComponent::setTextures(const std::vector<SDL_Texture*>& textures)
{
	textures_ = textures;
	if(textures_.size() > 0)
	{
		//Set the active texture to first frame
		currentFrame_ = 0.0f;
		setTexture(textures_[0]);
	}
}