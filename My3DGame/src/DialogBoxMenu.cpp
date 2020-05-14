//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "DialogBoxMenu.h"
#include "Renderer.h"

DialogBoxMenu::DialogBoxMenu(Game* game, const std::string& text, std::function<void()> onOK): UIScreen(game)
{
	//Adjust positions for dialog box
	bGPos_ = Vector2(0.0f, 0.0f);
	titlePos_ = Vector2(0.0f, 100.0f);
	nextButtonPos_ = Vector2(0.0f, 0.0f);
	background_ = game_->renderer()->getTexture("Assets/DialogBG.png");
	setTitle(text, Vector3(), 30);
	addButton("OKButton", [onOK]() {
		onOK();
	});
	addButton("CancelButton", [this]() {
		close();
	});
}
DialogBoxMenu::~DialogBoxMenu()
{
	//EMPTY:
}