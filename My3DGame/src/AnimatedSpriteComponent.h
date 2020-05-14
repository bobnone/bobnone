//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include <vector>
#include <SDL/SDL.h>
#include "SpriteComponent.h"

class AnimatedSpriteComponent: public SpriteComponent
{
public:
	AnimatedSpriteComponent(class Actor* owner, int drawOrder = 100);
	//Update animation every frame (overriden from component)
	void update(float deltaTime) override;
	//Set the textures used for animation
	void setTextures(const std::vector<SDL_Texture*>& textures);
	//Set/get the animation FPS
	float FPS() const
	{
		return FPS_;
	}
	void setFPS(float fps)
	{
		FPS_ = fps;
	}
private:
	//All textures in the animation
	std::vector<SDL_Texture*> textures_;
	//Current frame displayed
	float currentFrame_;
	//Animation frame rate
	float FPS_;
};