#pragma once
#include <string>
#include <GL/glew.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#include <SDL/SDL.h>
#include "Texture.h"

class TextureHandler
{
public:
	TextureHandler();
	~TextureHandler();
	bool Initialize();
	bool Screenshot();
protected:
	//
private:
	//
};