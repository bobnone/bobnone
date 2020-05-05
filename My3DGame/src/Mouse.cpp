// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Mouse.h"
#include "Game.h"
#include "Renderer.h"

Mouse::Mouse(Game* game):mGame(game)
{
	//
}
Mouse::~Mouse()
{
	//
}
void Mouse::Update(float deltaTime)
{
	//
}
void Mouse::ProcessInput(const uint8_t* keys)
{
	// Get position of mouse
	int x, y;
	SDL_GetMouseState(&x, &y);
	// Convert to (0,0) center coordinates
	vector2 mousePos(static_cast<float>(x), static_cast<float>(y));
	mousePos.x -= mGame->GetRenderer()->GetScreenWidth() * 0.5f;
	mousePos.y = mGame->GetRenderer()->GetScreenHeight() * 0.5f - mousePos.y;
}
void Mouse::HandleKeyPress(int key)
{
	switch (key)
	{
	case SDL_BUTTON_LEFT:
		//
		break;
	case SDL_BUTTON_RIGHT:
		//
		break;
	default:
		break;
	}
}
void Mouse::SetRelativeMouseMode(bool relative)
{
	if (relative)
	{
		SDL_SetRelativeMouseMode(SDL_TRUE);
		// Make an initial call to get relative to clear out
		SDL_GetRelativeMouseState(nullptr, nullptr);
	}
	else
	{
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}
}