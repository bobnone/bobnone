// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <string>
#include <functional>
#include "Game.h"
#include "UIScreen.h"

class DialogBoxMenu: public UIScreen
{
public:
	// (Lower draw order corresponds with further back)
	DialogBoxMenu(class Game* game, const std::string& text, std::function<void()> onOK);
	~DialogBoxMenu();
};