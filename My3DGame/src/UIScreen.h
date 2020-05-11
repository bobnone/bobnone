//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

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
	//UIScreen subclasses can override these
	virtual void update(float deltaTime);
	virtual void draw(class Shader* shader);
	virtual void processInput(const uint8_t* keys);
	virtual void handleKeyPress(int key);
	//Tracks if the UI is active or closing
	enum UIState
	{
		UI_ACTIVE,
		UI_CLOSING
	};
	//Set state to closing
	void close();
	//Get state of UI screen
	UIState state() const
	{
		return state_;
	}
	//Change the title text
	void setTitle(const std::string& text, const vector3& color = Color::White, int pointSize = 40);
	//Add a button to this screen
	void addButton(const std::string& name, std::function<void()> onClick);
protected:
	//Helper to draw a texture
	void drawTexture(class Shader* shader, class Texture* texture, const vector2& offset = vector2::Zero, float scale = 1.0f, bool flipY = false);
	class Game* game_;
	class Font* font_;
	class Texture* title_;
	class Texture* background_;
	//Configure positions
	vector2 titlePos_;
	vector2 nextButtonPos_;
	vector2 bGPos_;
	//State
	UIState state_;
	//List of buttons
	std::vector<class Button*> buttons_;
};