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
	errorCheck();
	//Initialize DevIL
	ilInit();
	errorCheck();
	//Initialize ILU
	iluInit();
	errorCheck();
	//Initialize ILUT
	ilutInit();
	errorCheck();
	//Tell DevIL to use English
	if(!iluSetLanguage(ILU_ENGLISH))
	{
		SDL_Log("DevIL failed to set \"English\" as its language!\n");
		return false;
	}
	errorCheck();
	//Tell ILUT to use OpenGL
	if(!ilutRenderer(ILUT_OPENGL))
	{
		SDL_Log("DevIL failed to enable OpenGL support!\n");
		return false;
	}
	errorCheck();
	/*When enabled, DevIL automatically converts paletted images to their base types, e.g. converting to a BGRA image.
	Note: Most modern graphics cards and OpenGL can't use palettes
	Note: Palettes are a really old convention that has been replaced by shaders*/
	if(!ilEnable(IL_CONV_PAL))
	{
		SDL_Log("DevIL failed to enable palette conversions!\n");
		return false;
	}
	errorCheck();
	//If enabled while saving, DevIL will overwrite existing files, else IL_FILE_ALREADY_EXISTS is set, and the image is not saved.
	if(!ilEnable(IL_FILE_OVERWRITE))
	{
		SDL_Log("DevIL failed to enable file overwriting!\n");
		return false;
	}
	errorCheck();
	//When enabled, the origin is specified at an absolute position, and all images loaded or saved adhere to this set origin. For more information, check out ilOriginFunc.
	if(!ilEnable(IL_ORIGIN_SET))
	{
		SDL_Log("DevIL failed to enable origin setting!\n");
		return false;
	}
	errorCheck();
	if(!ilOriginFunc(IL_ORIGIN_UPPER_LEFT))
	{
		SDL_Log("DevIL failed to set image origin!\n");
		return false;
	}
	errorCheck();
	//Disable on-board (hardware) image conversion, allowing DevIL to handle everything.
	if(!ilutEnable(ILUT_OPENGL_CONV))
	{
		SDL_Log("DevIL failed take over image conversions!\n");
		return false;
	}
	errorCheck();
	return true;
}
void TextureHandler::errorCheck()
{
	ILenum error;
	while ((error = ilGetError()) != IL_NO_ERROR)
	{
		switch (error)
		{
		case IL_COULD_NOT_OPEN_FILE:
			SDL_Log("DevIL Error \"%d\": %s", error, "Could not open the file specified. The file may already be open by another app or may not exist.");
			break;
		case IL_FILE_ALREADY_EXISTS:
			SDL_Log("DevIL Error \"%d\": %s", error, "The filename specified already belongs to another file. To overwrite files by default read more at ilEnable function.");
			break;
		case IL_FORMAT_NOT_SUPPORTED:
			SDL_Log("DevIL Error \"%d\": %s", error, "The format a function tried to use was not able to be used by that function.");
			break;
		case IL_ILLEGAL_FILE_VALUE:
			SDL_Log("DevIL Error \"%d\": %s", error, "An illegal value was found in a file trying to be loaded.");
			break;
		case IL_ILLEGAL_OPERATION:
			SDL_Log("DevIL Error \"%d\": %s", error, "The operation attempted is not allowable in the current state.");
			break;
		case IL_INTERNAL_ERROR:
			SDL_Log("DevIL Error \"%d\": %s", error, "A serious error/bug has occurred within DevIL.");
			break;
		case IL_INVALID_CONVERSION:
			SDL_Log("DevIL Error \"%d\": %s", error, "An invalid conversion attempt was tried.");
			break;
		case IL_INVALID_ENUM:
			SDL_Log("DevIL Error \"%d\": %s", error, "An unacceptable enumerated value was passed to a function.");
			break;
		case IL_INVALID_EXTENSION:
			SDL_Log("DevIL Error \"%d\": %s", error, "The extension of the specified filename was not correct for the type of image loading function.");
			break;
		case IL_INVALID_FILE_HEADER:
			SDL_Log("DevIL Error \"%d\": %s", error, "A file's header was incorrect.");
			break;
		case IL_INVALID_PARAM:
			SDL_Log("DevIL Error \"%d\": %s", error, "An invalid parameter was passed to a function, such as a NULL pointer.");
			break;
		case IL_INVALID_VALUE:
			SDL_Log("DevIL Error \"%d\": %s", error, "An invalid value was passed to a function or was in a file.");
			break;
		case IL_LIB_JPEG_ERROR:
			SDL_Log("DevIL Error \"%d\": %s", error, "An error occurred in the libjpeg library.");
			break;
		case IL_LIB_PNG_ERROR:
			SDL_Log("DevIL Error \"%d\": %s", error, "An error occurred in the libpng library.");
			break;
		case IL_OUT_FORMAT_SAME:
			SDL_Log("DevIL Error \"%d\": %s", error, "Tried to convert an image from its format to the same format.");
			break;
		case IL_OUT_OF_MEMORY:
			SDL_Log("DevIL Error \"%d\": %s", error, "Could not allocate enough memory in an operation.");
			break;
		case IL_STACK_OVERFLOW:
			SDL_Log("DevIL Error \"%d\": %s", error, "One of the internal stacks was already filled, and the user tried to add on to the full stack.");
			break;
		case IL_STACK_UNDERFLOW:
			SDL_Log("DevIL Error \"%d\": %s", error, "One of the internal stacks was empty, and the user tried to empty the already empty stack.");
			break;
		case ILUT_NOT_SUPPORTED:
			SDL_Log("DevIL Error \"%d\": %s", error, "A type is valid but not supported in the current build.");
			break;
		default:
			SDL_Log("DevIL Error \"%d\": %s", error, "Unknown Error!");
			break;
		}
	}
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