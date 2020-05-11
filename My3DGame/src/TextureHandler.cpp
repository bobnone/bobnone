#include "TextureHandler.h"

TextureHandler::TextureHandler()
{
	//EMPTY:
}
TextureHandler::~TextureHandler()
{
	//EMPTY:
}
bool TextureHandler::initialize()
{
	//Version checking
	if(ilGetInteger(IL_VERSION_NUM) < IL_VERSION)
	{
		SDL_Log("DevIL IL version is different!\n");
		return false;
	}
	else if(iluGetInteger(ILU_VERSION_NUM) < ILU_VERSION)
	{
		SDL_Log("DevIL ILU version is different!\n");
		return false;
	}
	else if(ilutGetInteger(ILUT_VERSION_NUM) < ILUT_VERSION)
	{
		SDL_Log("DevIL ILUT version is different!\n");
		return false;
	}
	//Initialize DevIL
	ilInit();
	//Initialize ILU
	iluInit();
	//Initialize ILUT
	ilutInit();
	//Tell DevIL to use English
	if(!iluSetLanguage(ILU_ENGLISH))
	{
		SDL_Log("DevIL failed to set \"English\" as its language!\n");
		return false;
	}
	//Tell ILUT to use OpenGL
	if(!ilutRenderer(ILUT_OPENGL))
	{
		SDL_Log("DevIL failed to enable OpenGL support!\n");
		return false;
	}
	/*When enabled, DevIL automatically converts paletted images to their base types, e.g. converting to a BGRA image.
	Note: Most modern graphics cards and OpenGL can't use palettes
	Note: Palettes are a really old convention that has been replaced by shaders*/
	if(!ilEnable(IL_CONV_PAL))
	{
		SDL_Log("DevIL failed to enable palette conversions!\n");
		return false;
	}
	//If enabled while saving, DevIL will overwrite existing files, else IL_FILE_ALREADY_EXISTS is set, and the image is not saved.
	if(!ilEnable(IL_FILE_OVERWRITE))
	{
		SDL_Log("DevIL failed to enable file overwriting!\n");
		return false;
	}
	//When enabled, the origin is specified at an absolute position, and all images loaded or saved adhere to this set origin. For more information, check out ilOriginFunc.
	if(!ilEnable(IL_ORIGIN_SET))
	{
		SDL_Log("DevIL failed to enable origin setting!\n");
		return false;
	}
	if(!ilOriginFunc(IL_ORIGIN_UPPER_LEFT))
	{
		SDL_Log("DevIL failed to set image origin!\n");
		return false;
	}
	//Disable on-board (hardware) image conversion, allowing DevIL to handle everything.
	if(!ilutEnable(ILUT_OPENGL_CONV))
	{
		SDL_Log("DevIL failed take over image conversions!\n");
		return false;
	}
	return true;
}
//Used to take a screenshot
ILboolean TextureHandler::screenshot()
{
	//TODO: decide how to do this
	//loads the viewport's data into the current bound image, for you to decide what to do with it. If you want to automate the process even further and save the viewport's data directly to disk, use:
	//return ilutGLScreen();
	//does not modify the current bound image at all.This function is very specialized and saves the image to a Targa file with the filename format of screen% d.tga, where% d is a number from 0 to 126. This function will probably not be suited to most developers' preferences.
	//return ilutGLScreenie();
	return true;
}