// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Math.h"
#include <cstdint>
#include <string>
#include <functional>
#include <vector>

class Mouse
{
public:
	Mouse(class Game* game);
	virtual ~Mouse();
	// Mouse subclasses can override these
	virtual void Update(float deltaTime);
	virtual void ProcessInput(const uint8_t* keys);
	virtual void HandleKeyPress(int key);
protected:
	// Sets the mouse mode to relative or not
	void SetRelativeMouseMode(bool relative);
	class Game* mGame;
};