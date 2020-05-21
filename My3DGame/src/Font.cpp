//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "Font.h"
#include "Texture.h"
#include "Game.h"

Font::Font(class Game* game): game_(game)
{
	//EMPTY:
}
Font::~Font()
{
	//EMPTY:
}
bool Font::load(const std::string& fileName)
{
	//We support these font sizes
	std::vector<int> fontSizes = {
		8, 9,
		10, 11, 12, 14, 16, 18,
		20, 22, 24, 26, 28,
		30, 32, 34, 36, 38,
		40, 42, 44, 46, 48,
		52, 56,
		60, 64, 68,
		72
	};
	for(auto& size: fontSizes)
	{
		TTF_Font* font = TTF_OpenFont(fileName.c_str(), size);
		if (font == nullptr)
		{
			SDL_Log("Failed to load font %s in size %d", fileName.c_str(), size);
			return false;
		}
		fontData_.emplace(size, font);
	}
	return true;
}
void Font::unload()
{
	for(auto& font : fontData_)
	{
		TTF_CloseFont(font.second);
	}
}
Texture* Font::renderText(const std::string& textKey, const Vector3& color, int pointSize)
{
	Texture* texture = nullptr;
	//Convert to SDL_Color
	SDL_Color sdlColor;
	sdlColor.r = static_cast<Uint8>(color.x * 255);
	sdlColor.g = static_cast<Uint8>(color.y * 255);
	sdlColor.b = static_cast<Uint8>(color.z * 255);
	sdlColor.a = 255;
	//Find the font data for this point size
	auto iter = fontData_.find(pointSize);
	if (iter != fontData_.end())
	{
		TTF_Font* font = iter->second;
		const std::string& actualText = game_->getText(textKey);
		//Draw this to a surface (blended for alpha)
		SDL_Surface* surf = TTF_RenderUTF8_Blended(font, actualText.c_str(), sdlColor);
		if(surf != nullptr)
		{
			//Convert from surface to texture
			texture = new Texture();
			texture->createFromSurface(surf);
			SDL_FreeSurface(surf);
		}
	}
	else
	{
		SDL_Log("Point size %d is unsupported", pointSize);
	}
	return texture;
}