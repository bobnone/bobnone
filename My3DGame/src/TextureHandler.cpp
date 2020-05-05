#include "TextureHandler.h"

TextureHandler::TextureHandler()
{
	//
}
TextureHandler::~TextureHandler()
{
	//
}
bool TextureHandler::Initialize()
{
	//Version checking
	if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION)
	{
		printf("DevIL IL version is different!\n");
		return false;
	}
	if (iluGetInteger(ILU_VERSION_NUM) < ILU_VERSION)
	{
		printf("DevIL ILU version is different!\n");
		return false;
	}
	if (ilutGetInteger(ILUT_VERSION_NUM) < ILUT_VERSION)
	{
		printf("DevIL ILUT version is different!\n");
		return false;
	}
	// Initialize DevIL
	ilInit();
	// Initialize ILU
	iluInit();
	// Initialize ILUT
	ilutInit();
	// Tell ILUT to use OpenGL
	ILboolean noError = ilutRenderer(ILUT_OPENGL);
	if (!noError)
	{
		printf("DevIL failed to enable OpenGL support!\n");
		return false;
	}
	//When enabled, DevIL automatically converts palette'd images to their base types, e.g. converting to a BGRA image.
	// Note: Most modern graphics cards and OpenGL can't use palettes
	// Note: Palettes are a really old convention that has been replaced by shaders
	noError = ilEnable(IL_CONV_PAL);
	if (!noError)
	{
		printf("DevIL failed to enable palette conversions!\n");
		return false;
	}
	//If enabled while saving, DevIL will overwrite existing files, else IL_FILE_ALREADY_EXISTS is set, and the image is not saved.
	noError = ilEnable(IL_FILE_OVERWRITE);
	if (!noError)
	{
		printf("DevIL failed to enable file overwriting!\n");
		return false;
	}
	//When enabled, the origin is specified at an absolute position, and all images loaded or saved adhere to this set origin. For more information, check out ilOriginFunc.
	noError = ilEnable(IL_ORIGIN_SET);
	if (!noError)
	{
		printf("DevIL failed to enable origin setting!\n");
		return false;
	}
	noError = ilOriginFunc(IL_ORIGIN_UPPER_LEFT);
	if (!noError)
	{
		printf("DevIL failed to set image origin!\n");
		return false;
	}
	// Disable on-board image conversion, allowing DevIL to handle everything.
	noError = ilutEnable(ILUT_OPENGL_CONV);
	if (!noError)
	{
		printf("DevIL failed take over image conversions!\n");
		return false;
	}
	return true;
}
bool TextureHandler::Screenshot()
{
	//ilutOglScreen loads the view-port's data into the current bound image, for you to decide what to do with it.
	//ILboolean ilutOglScreen();
	//ilutGLScreen copies the current OpenGL window contents to the current bound image, effectively taking a screen-shot. The new image attributes are that of the current OpenGL window's.
	//ILboolean ilutGLScreen();
	return false;
}