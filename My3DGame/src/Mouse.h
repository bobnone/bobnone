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
	void ProcessInput(const uint8_t* keys);
	void HandleKeyPress(int key);
	// Shows/hides the mouse pointer
	void SetVisable(bool visable)
	{
		mVisable = visable;
	}
	// Sets the mouse mode to relative or not
	void SetRelative(bool relative);
private:
	class Game* mGame;
	vector2 mPosition;
	bool mVisable;
	bool mRelative;
};