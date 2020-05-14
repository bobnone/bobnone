//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include <string>
#include <unordered_map>
#include <SDL/SDL_ttf.h>
#include "Color.h"

class Font
{
public:
	Font(class Game* game);
	~Font();
	//Load/unload from a file
	bool load(const std::string& fileName);
	void unload();
	//Given string and this font, draw to a texture
	class Texture* renderText(const std::string& textKey, const Vector3& color = Color::White, int pointSize = 30);
private:
	//Map of point sizes to font data
	std::unordered_map<int, TTF_Font*> fontData_;
	class Game* game_;
};