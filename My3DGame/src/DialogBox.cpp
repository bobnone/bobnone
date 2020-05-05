// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "DialogBox.h"
#include "Game.h"
#include "Renderer.h"

DialogBox::DialogBox(Game* game, const std::string& text, std::function<void()> onOK) :UIScreen(game)
{
	// Adjust positions for dialog box
	mBGPos = vector2(0.0f, 0.0f);
	mTitlePos = vector2(0.0f, 100.0f);
	mNextButtonPos = vector2(0.0f, 0.0f);
	mBackground = mGame->GetTexture("Assets/DialogBG.png");
	SetTitle(text, vector3::Zero, 30);
	AddButton("OKButton", [onOK]() {
		onOK();
	});
	AddButton("CancelButton", [this]() {
		Close();
	});
}
DialogBox::~DialogBox()
{
	//
}