//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include <SDL/SDL_scancode.h>
#include <SDL/SDL_gamecontroller.h>
#include <SDL/SDL_mouse.h>
#include "Math/Math.h"

//The different button states
enum ButtonState
{
	ENone,
	EPressed,
	EReleased,
	EHeld
};

//Helper for keyboard input
class KeyboardState
{
public:
	//Friend so InputSystem can easily update it
	friend class InputSystem;
	//Get just the boolean true/false value of key
	bool getKeyValue(SDL_Scancode keyCode) const;
	//Get a state based on current and previous frame
	ButtonState getKeyState(SDL_Scancode keyCode) const;
private:
	const Uint8* currState_;
	Uint8 prevState_[SDL_NUM_SCANCODES];
};

//Helper for mouse input
class MouseState
{
public:
	friend class InputSystem;

	//For mouse position
	const Vector2& getPosition() const
	{
		return mousePos_;
	}
	const Vector2& scrollWheel() const
	{
		return scrollWheel_;
	}
	bool isRelative() const
	{
		return isRelative_;
	}
	//For buttons
	bool getButtonValue(int button) const;
	ButtonState getButtonState(int button) const;
private:
	//Store current mouse position
	Vector2 mousePos_;
	//Motion of scroll wheel
	Vector2 scrollWheel_;
	//Store button data
	Uint32 currButtons_;
	Uint32 prevButtons_;
	//Are we in relative mouse mode
	bool isRelative_;
};

//Helper for controller input
class ControllerState
{
public:
	friend class InputSystem;
	//For buttons
	bool getButtonValue(SDL_GameControllerButton button) const;
	ButtonState getButtonState(SDL_GameControllerButton button) const;
	const Vector2& leftStick() const
	{
		return leftStick_;
	}
	const Vector2& rightStick() const
	{
		return rightStick_;
	}
	float leftTrigger() const
	{
		return leftTrigger_;
	}
	float rightTrigger() const
	{
		return rightTrigger_;
	}
	bool isConnected() const
	{
		return isConnected_;
	}
private:
	//Current/previous buttons
	Uint8 currButtons_[SDL_CONTROLLER_BUTTON_MAX];
	Uint8 prevButtons_[SDL_CONTROLLER_BUTTON_MAX];
	//Left/right sticks
	Vector2 leftStick_;
	Vector2 rightStick_;
	//Left/right trigger
	float leftTrigger_;
	float rightTrigger_;
	//Is this controller connected?
	bool isConnected_;
};
//Wrapper that contains current state of input
struct InputState
{
	KeyboardState Keyboard;
	MouseState Mouse;
	ControllerState Controller;
};
class InputSystem
{
public:
	bool initialize();
	void shutdown();
	//Called right before SDL_PollEvents loop
	void prepareForUpdate();
	//Called after SDL_PollEvents loop
	void update();
	//Called to process an SDL event in input system
	void processEvent(union SDL_Event& event);
	const InputState& state() const
	{
		return state_;
	}
	void setRelativeMouseMode(bool value);
private:
	float filter1D(int input);
	Vector2 filter2D(int inputX, int inputY);
	InputState state_;
	SDL_GameController* controller_;
};