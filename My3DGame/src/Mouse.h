#pragma once
#include <cstdint>
#include "Math.h"
#include "Game.h"
#include "Shader.h"

class Mouse
{
public:
	Mouse(class Game* game);
	~Mouse();
	void update(float deltaTime);
	void processInput(SDL_Event* event);
	//Returns the mouse's position vector
	vector2 position()
	{
		return position_;
	}
	//Returns a vector representing the change in position
	vector2 getPositionChange()
	{
		return change_;
	}
	//Sets the mouse mode to relative or not
	void setRelative(bool relative);
	//Returns if the mouse mode is relative or not
	bool relative()
	{
		return relative_;
	}
	//Returns if the left mouse button is clicked or not
	bool clicked()
	{
		return clicked_;
	}
private:
	class Game* game_;
	vector2 position_;
	vector2 old_;
	vector2 change_;
	bool relative_;
	bool clicked_;
};