//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#define ILUT_USE_OPENGL

#include "Game.h"

int main(int argc, char** argv)
{
	Game game;
	if(game.initialize())
	{
		game.runLoop();
	}
	game.shutdown();
	return 0;
}