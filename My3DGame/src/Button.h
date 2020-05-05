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
#include "Math.h"
#include "Shader.h"
#include "Game.h"

class Button
{
public:
	// Tracks the Button's current state
	enum ButtonState
	{
		NORMAL = 0,
		HOVER,
		CLICKED
	};
	Button(const std::string& name, class Game* game, class Font* font, std::function<void()> onClick, const vector2& pos);
	~Button();
	// Getters/setters
	class Texture* GetNameTexture()
	{
		return mTexture;
	}
	class Texture* GetButtonTexure();
	const vector2& GetPosition() const
	{
		return mPosition;
	}
	const vector2& GetDimensions() const
	{
		return mDimensions;
	}
	void Update(float deltaTime);
	void Draw(class Shader* shader);
	void DrawTexture(class Shader* shader, class Texture* texture);
	void SetState(ButtonState bs)
	{
		mState = bs;
	}
	ButtonState GetState() const
	{
		return mState;
	}
	bool IsHighlighted() const
	{
		return mState == HOVER;
	}
	bool IsClicked() const
	{
		return mState == CLICKED;
	}
	// Handles mouse positioion
	void ProcessInput(const vector2& mousePos);
	// Returns true if the point is within the button's bounds
	bool ContainsPoint(const vector2& pt) const;
	// Called when button is clicked
	void OnClick();
private:
	std::function<void()> mOnClick;
	std::string mName;
	class Texture* mTexture;
	class Texture* mButtonNormal;
	class Texture* mButtonHover;
	class Texture* mButtonClicked;
	class Font* mFont;
	vector2 mPosition;
	vector2 mDimensions;
	ButtonState mState;
	// Set the name of the button
	void SetName(const std::string& name);
};