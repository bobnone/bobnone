//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "InputSystem.h"
#include <SDL/SDL.h>
#include <cstring>

bool KeyboardState::getKeyValue(SDL_Scancode keyCode) const
{
	return currState_[keyCode] == 1;
}
ButtonState KeyboardState::getKeyState(SDL_Scancode keyCode) const
{
	if(prevState_[keyCode] == 0)
	{
		if(currState_[keyCode] == 0)
		{
			return ENone;
		}
		else
		{
			return EPressed;
		}
	}
	else
	{
		if(currState_[keyCode] == 0)
		{
			return EReleased;
		}
		else
		{
			return EHeld;
		}
	}
}
bool MouseState::getButtonValue(int button) const
{
	return (SDL_BUTTON(button) & currButtons_) == 1;
}
ButtonState MouseState::getButtonState(int button) const
{
	int mask = SDL_BUTTON(button);
	if((mask & prevButtons_) == 0)
	{
		if((mask & currButtons_) == 0)
		{
			return ENone;
		}
		else
		{
			return EPressed;
		}
	}
	else
	{
		if((mask & currButtons_) == 0)
		{
			return EReleased;
		}
		else
		{
			return EHeld;
		}
	}
}
bool ControllerState::getButtonValue(SDL_GameControllerButton button) const
{
	return currButtons_[button] == 1;
}
ButtonState ControllerState::getButtonState(SDL_GameControllerButton button) const
{
	if(prevButtons_[button] == 0)
	{
		if(currButtons_[button] == 0)
		{
			return ENone;
		}
		else
		{
			return EPressed;
		}
	}
	else
	{
		if(currButtons_[button] == 0)
		{
			return EReleased;
		}
		else
		{
			return EHeld;
		}
	}
}
bool InputSystem::initialize()
{
	//Keyboard
	//Assign current state pointer
	state_.Keyboard.currState_ = SDL_GetKeyboardState(NULL);
	//Clear previous state memory
	memset(state_.Keyboard.prevState_, 0,SDL_NUM_SCANCODES);
	//Mouse (just set everything to 0)
	state_.Mouse.currButtons_ = 0;
	state_.Mouse.prevButtons_ = 0;
	//Get the connected controller, if it exists
	controller_ = SDL_GameControllerOpen(0);
	//Initialize controller state
	state_.Controller.isConnected_ = (controller_ != nullptr);
	memset(state_.Controller.currButtons_, 0, SDL_CONTROLLER_BUTTON_MAX);
	memset(state_.Controller.prevButtons_, 0, SDL_CONTROLLER_BUTTON_MAX);
	return true;
}
void InputSystem::shutdown()
{
	//EMPTY:
}
void InputSystem::prepareForUpdate()
{
	//Copy current state to previous
	//Keyboard
	memcpy(state_.Keyboard.prevState_, state_.Keyboard.currState_, SDL_NUM_SCANCODES);
	//Mouse
	state_.Mouse.prevButtons_ = state_.Mouse.currButtons_;
	state_.Mouse.isRelative_ = false;
	state_.Mouse.scrollWheel_ = vector2::Zero;
	//Controller
	memcpy(state_.Controller.prevButtons_, state_.Controller.currButtons_, SDL_CONTROLLER_BUTTON_MAX);
}
void InputSystem::update()
{
	//Mouse
	int x = 0, y = 0;
	if(state_.Mouse.isRelative_)
	{
		state_.Mouse.currButtons_ = SDL_GetRelativeMouseState(&x, &y);
	}
	else
	{
		state_.Mouse.currButtons_ = SDL_GetMouseState(&x, &y);
	}
	state_.Mouse.mousePos_.x = static_cast<float>(x);
	state_.Mouse.mousePos_.y = static_cast<float>(y);
	//Controller:
	//Buttons
	for(int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++)
	{
		state_.Controller.currButtons_[i] = SDL_GameControllerGetButton(controller_, SDL_GameControllerButton(i));
	}
	//Triggers
	state_.Controller.leftTrigger_ = filter1D(SDL_GameControllerGetAxis(controller_, SDL_CONTROLLER_AXIS_TRIGGERLEFT));
	state_.Controller.rightTrigger_ = filter1D(SDL_GameControllerGetAxis(controller_, SDL_CONTROLLER_AXIS_TRIGGERRIGHT));
	//Sticks
	x = SDL_GameControllerGetAxis(controller_, SDL_CONTROLLER_AXIS_LEFTX);
	y = -SDL_GameControllerGetAxis(controller_, SDL_CONTROLLER_AXIS_LEFTY);
	state_.Controller.leftStick_ = filter2D(x, y);
	x = SDL_GameControllerGetAxis(controller_, SDL_CONTROLLER_AXIS_RIGHTX);
	y = -SDL_GameControllerGetAxis(controller_, SDL_CONTROLLER_AXIS_RIGHTY);
	state_.Controller.rightStick_ = filter2D(x, y);
}
void InputSystem::processEvent(SDL_Event& event)
{
	switch(event.type)
	{
	case SDL_MOUSEWHEEL:
		state_.Mouse.scrollWheel_ = vector2(static_cast<float>(event.wheel.x), static_cast<float>(event.wheel.y));
		break;
	default:
		break;
	}
}
void InputSystem::setRelativeMouseMode(bool value)
{
	SDL_bool set = value ? SDL_TRUE:SDL_FALSE;
	SDL_SetRelativeMouseMode(set);
	state_.Mouse.isRelative_ = value;
}
float InputSystem::filter1D(int input)
{
	//A value < dead zone is interpreted as 0%
	const int deadZone = 250;
	//A value > max value is interpreted as 100%
	const int maxValue = 30000;
	float retVal = 0.0f;
	//Take absolute value of input
	int absValue = input > 0 ? input : -input;
	//Ignore input within dead zone
	if(absValue > deadZone)
	{
		//Compute fractional value between dead zone and max value
		retVal = static_cast<float>(absValue - deadZone) / (maxValue - deadZone);
		//Make sure sign matches original value
		retVal = input > 0 ? retVal : -1.0f * retVal;
		//Clamp between -1.0f and 1.0f
		retVal = Math::Clamp(retVal, -1.0f, 1.0f);
	}
	return retVal;
}
vector2 InputSystem::filter2D(int inputX, int inputY)
{
	const float deadZone = 8000.0f;
	const float maxValue = 30000.0f;
	//Make into 2D vector
	vector2 dir;
	dir.x = static_cast<float>(inputX);
	dir.y = static_cast<float>(inputY);
	float length = dir.Length();
	//If length < deadZone, should be no input
	if(length < deadZone)
	{
		dir = vector2::Zero;
	}
	else
	{
		//Calculate fractional value between dead zone and max value circles
		float f = (length - deadZone) / (maxValue - deadZone);
		//Clamp f between 0.0f and 1.0f
		f = Math::Clamp(f, 0.0f, 1.0f);
		//Normalize the vector, and then scale it to the fractional value
		dir *= f / length;
	}
	return dir;
}