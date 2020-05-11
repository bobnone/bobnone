#pragma once
#include <string>
#include <functional>
#include "Math.h"
#include "Shader.h"
#include "Game.h"

class Button
{
public:
	//Tracks the Button's current state
	enum ButtonState
	{
		BUTTON_NORMAL = 0,
		BUTTON_HOVER,
		BUTTON_CLICKED
	};
	Button(const std::string& name, class Game* game, class Font* font, std::function<void()> onClick, const vector2& pos);
	~Button();
	//Getters/setters
	class Texture* getNameTexture()
	{
		return texture_;
	}
	class Texture* getButtonTexure();
	const vector2& position() const
	{
		return position_;
	}
	const vector2& dimensions() const
	{
		return dimensions_;
	}
	void update(float deltaTime);
	void draw(class Shader* shader);
	void drawTexture(class Shader* shader, class Texture* texture);
	void setState(ButtonState bs)
	{
		state_ = bs;
	}
	ButtonState state() const
	{
		return state_;
	}
	bool isHighlighted() const
	{
		return state_ == BUTTON_HOVER;
	}
	bool isClicked() const
	{
		return state_ == BUTTON_CLICKED;
	}
	//Handles mouse positions
	void processInput(const vector2& mousePos);
	//Returns true if the point is within the button's bounds
	bool containsPoint(const vector2& pt) const;
	//Called when button is clicked
	void onClick();
private:
	std::function<void()> f_onClick_;
	std::string name_;
	class Texture* texture_;
	class Texture* buttonNormal_;
	class Texture* buttonHover_;
	class Texture* buttonClicked_;
	class Font* font_;
	vector2 position_;
	vector2 dimensions_;
	ButtonState state_;
	//Set the name of the button
	void setName(const std::string& name);
};