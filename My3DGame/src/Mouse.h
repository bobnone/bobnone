 // ----------------------------------------------------------------
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
	void Update(float deltaTime);
	void ProcessInput(SDL_Event* event);
	// Returns the mouse's position vector
	vector2 GetPosition()
	{
		return mPosition;
	}
	// Returns a vector representing the change in position
	vector2 GetPositionChange()
	{
		return mChange;
	}
	// Sets the mouse mode to relative or not
	void SetRelative(bool relative);
	// Returns if the mouse mode is relative or not
	bool IsRelative()
	{
		return mRelative;
	}
	// Returns if the left mouse button is clicked or not
	bool Clicked()
	{
		return mClicked;
	}
private:
	class Game* mGame;
	vector2 mPosition;
	vector2 mOld;
	vector2 mChange;
	bool mRelative;
	bool mClicked;
};