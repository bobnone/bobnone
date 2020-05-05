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
#include <vector>
#include "Game.h"
#include "Texture.h"
#include "Shader.h"
#include "Font.h"
#include "Math.h"
#include "Color.h"
#include "Button.h"

class UIScreen
{
public:
	UIScreen(class Game* game);
	virtual ~UIScreen();
	// UIScreen subclasses can override these
	virtual void Update(float deltaTime);
	virtual void Draw(class Shader* shader);
	virtual void ProcessInput(const uint8_t* keys);
	virtual void HandleKeyPress(int key);
	// Tracks if the UI is active or closing
	enum UIState
	{
		UI_ACTIVE,
		UI_CLOSING
	};
	// Set state to closing
	void Close();
	// Get state of UI screen
	UIState GetState() const
	{
		return mState;
	}
	// Change the title text
	void SetTitle(const std::string& text, const vector3& color = Color::White, int pointSize = 40);
	// Add a button to this screen
	void AddButton(const std::string& name, std::function<void()> onClick);
protected:
	// Helper to draw a texture
	void DrawTexture(class Shader* shader, class Texture* texture, const vector2& offset = vector2::Zero, float scale = 1.0f, bool flipY = false);
	class Game* mGame;
	class Font* mFont;
	class Texture* mTitle;
	class Texture* mBackground;
	// Configure positions
	vector2 mTitlePos;
	vector2 mNextButtonPos;
	vector2 mBGPos;
	// State
	UIState mState;
	// List of buttons
	std::vector<class Button*> mButtons;
};