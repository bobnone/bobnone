#include "Mouse.h"
#include "Game.h"
#include "Renderer.h"

Mouse::Mouse(Game* game):mGame(game), mPosition(0.0f,0.0f), mOld(0.0f, 0.0f), mChange(0.0f, 0.0f), mRelative(false), mClicked(false)
{
	//
}
Mouse::~Mouse()
{
	//
}
void Mouse::Update(float deltaTime)
{
	// Get position of mouse
	int x, y;
	SDL_GetMouseState(&x, &y);
	// Convert to (0,0) center coordinates
	mPosition = vector2(static_cast<float>(x), static_cast<float>(y));
	mPosition.x -= mGame->GetRenderer()->GetScreenWidth() * 0.5f;
	mPosition.y = mGame->GetRenderer()->GetScreenHeight() * 0.5f - mPosition.y;
	// Calculate the change in position
	if (mOld != mPosition)
	{
		mChange.x = mOld.x - mPosition.x;
		mChange.y = mOld.y - mPosition.y;
		mOld = mPosition;
	}
}
void Mouse::ProcessInput(SDL_Event* event)
{
	switch (event->type)
	{
	case SDL_MOUSEBUTTONDOWN:
		switch (event->button.button)
		{
		case SDL_BUTTON_LEFT:
			mClicked = true;
			break;
		case SDL_BUTTON_MIDDLE:
			//
			break;
		case SDL_BUTTON_RIGHT:
			//
			break;
		default:
			break;
		}
		break;
	case SDL_MOUSEBUTTONUP:
		switch (event->button.button)
		{
		case SDL_BUTTON_LEFT:
			mClicked = false;
			break;
		case SDL_BUTTON_MIDDLE:
			//
			break;
		case SDL_BUTTON_RIGHT:
			//
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}
void Mouse::SetRelative(bool relative)
{
	if(relative)
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