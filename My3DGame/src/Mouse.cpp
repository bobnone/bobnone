#include "Mouse.h"
#include "Game.h"
#include "Renderer.h"

Mouse::Mouse(Game* game):game_(game), position_(0.0f,0.0f), old_(0.0f, 0.0f), change_(0.0f, 0.0f), relative_(false), clicked_(false)
{
	//EMPTY:
}
Mouse::~Mouse()
{
	//EMPTY:
}
void Mouse::update(float deltaTime)
{
	//Get position of mouse
	int x, y;
	SDL_GetMouseState(&x, &y);
	//Convert to (0,0) center coordinates
	position_ = Vector2(static_cast<float>(x), static_cast<float>(y));
	position_.x -= game_->renderer()->screenWidth() * 0.5f;
	position_.y = game_->renderer()->screenHeight() * 0.5f - position_.y;
	//Calculate the change in position
	if(old_ != position_)
	{
		change_.x = old_.x - position_.x;
		change_.y = old_.y - position_.y;
		old_ = position_;
	}
}
void Mouse::processInput(SDL_Event* event)
{
	switch(event->type)
	{
	case SDL_MOUSEBUTTONDOWN:
		switch(event->button.button)
		{
		case SDL_BUTTON_LEFT:
			clicked_ = true;
			break;
		case SDL_BUTTON_MIDDLE:
			//EMPTY:
			break;
		case SDL_BUTTON_RIGHT:
			//EMPTY:
			break;
		default:
			break;
		}
		break;
	case SDL_MOUSEBUTTONUP:
		switch(event->button.button)
		{
		case SDL_BUTTON_LEFT:
			clicked_ = false;
			break;
		case SDL_BUTTON_MIDDLE:
			//EMPTY:
			break;
		case SDL_BUTTON_RIGHT:
			//EMPTY:
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}
void Mouse::setRelative(bool relative)
{
	if(relative)
	{
		SDL_SetRelativeMouseMode(SDL_TRUE);
		//Make an initial call to get relative to clear out
		SDL_GetRelativeMouseState(nullptr, nullptr);
	}
	else
	{
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}
}