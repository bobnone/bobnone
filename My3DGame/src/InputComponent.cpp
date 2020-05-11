//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "InputComponent.h"
#include "Actor.h"
#include "InputSystem.h"

InputComponent::InputComponent(class Actor* owner):MoveComponent(owner), forwardKey_(0), backKey_(0), clockwiseKey_(0), counterClockwiseKey_(0)
{
	//EMPTY:
}
void InputComponent::processInput(const InputState& state)
{
	//Calculate forward speed for MoveComponent
	float forwardSpeed = 0.0f;
	if(state.Keyboard.getKeyValue(SDL_Scancode(forwardKey_)))
	{
		forwardSpeed += maxForwardSpeed_;
	}
	if(state.Keyboard.getKeyValue(SDL_Scancode(backKey_)))
	{
		forwardSpeed -= maxForwardSpeed_;
	}
	setForwardSpeed(forwardSpeed);
	//Calculate angular speed for MoveComponent
	float angularSpeed = 0.0f;
	if(state.Keyboard.getKeyValue(SDL_Scancode(clockwiseKey_)))
	{
		angularSpeed += maxAngularSpeed_;
	}
	if(state.Keyboard.getKeyValue(SDL_Scancode(counterClockwiseKey_)))
	{
		angularSpeed -= maxAngularSpeed_;
	}
	setAngularXSpeed(angularSpeed);
}