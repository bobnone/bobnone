#include "Texture.h"
#include <locale>
#include <codecvt>
#include <iostream>

Texture::Texture():fileName_(""), textureID_(0), imageID_(0), info_(nullptr), width_(0), height_(0), format_(GL_RGBA)
{
	//EMPTY:
}
bool Texture::load(const std::string& fileName)
{
	fileName_ = fileName;
	//Creating a blank image to work with
	ilGenImages(1, &imageID_);
	if(errorCheck())
	{
		return false;
	}
	//Bind image so that DevIL performs all subsequent operations on this image
	ilBindImage(imageID_);
	if(errorCheck())
	{
		return false;
	}
	if(!loadImage((ILstring)fileName.c_str()))
	{
		SDL_Log("DevIL failed to load image: \"%s\"", fileName.c_str());
		//Unbind the current image in case the current image is this image
		ilBindImage(NULL);
		errorCheck();
		//Delete the DevIL image
		ilDeleteImages(1, &imageID_);
		errorCheck();
		return false;
	}
	if(errorCheck())
	{
		return false;
	}
	if(!iluBuildMipmaps())
	{
		SDL_Log("DevIL failed to build mipmaps for: \"%s\"", fileName.c_str());
		//Unbind the current image in case the current image is this image
		ilBindImage(NULL);
		errorCheck();
		//Delete the DevIL image
		ilDeleteImages(1, &imageID_);
		errorCheck();
		return false;
	}
	if(errorCheck())
	{
		return false;
	}
	//Note: data pointer deleted when the image is deleted
	ILubyte* data = ilGetData();
	if(errorCheck())
	{
		return false;
	}
	//create an empty ILinfo struct.
	info_ = new ILinfo();
	//Retrieves information about the current image in an ILinfo struct.
	iluGetImageInfo(info_);
	if (errorCheck())
	{
		return false;
	}
	width_ = info_->Width;
	height_ = info_->Height;
	format_ = info_->Format;
	glGenTextures(1, &textureID_);
	glBindTexture(GL_TEXTURE_2D, textureID_);
	/*Note: It has been reported that on some ATI drivers, glGenerateMipmap(GL_TEXTURE_2D) has no effect unless you precede it with a call to glEnable(GL_TEXTURE_2D) in this particular case.
	Once again, to be clear, bind the texture, glEnable, then glGenerateMipmap.
	This is a bug and has been in the ATI drivers for a while.
	Perhaps by the time you read this, it will have been corrected. (glGenerateMipmap doesn't work on ATI as of 2011)*/
	glEnable(GL_TEXTURE_2D);
	//Have DevIL generate bind, and create an OpenGL texture.
	//Note: This is done via glGenTextures, glBindTexture, glTexImage2D, and gluBuild2DMipmaps after performing any required conversions.
	ilutGLTexImage(0);
	//textureID_ = ilutGLBindTexImage();
	if(errorCheck())
	{
		return false;
	}
	//Texture created?
	if(textureID_ == 0)
	{
		SDL_Log("DevIL failed to create an OpenGL texture for: \"%s\"", fileName.c_str());
		//Unbind the current image in case the current image is this image
		ilBindImage(NULL);
		errorCheck();
		//Delete the DevIL image
		ilDeleteImages(1, &imageID_);
		errorCheck();
		//Delete the OpenGL texture
		glDeleteTextures(1, &textureID_);
		errorCheck();
		if(info_ != NULL)
		{
			delete info_;
			info_ = NULL;
		}
		return false;
	}
	// Enable linear filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Set texture wrapping:
	// Note: default is GL_REPEAT
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// Enable anisotropic filtering, if supported
	if(GLEW_EXT_texture_filter_anisotropic)
	{
		// Get the maximum anisotropy value
		GLfloat largest;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &largest);
		// Enable it
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, largest);
	}
	return true;
}
bool Texture::oldLoad(const std::string& fileName)
{
	fileName_ = fileName;
	// Creating a blank image to work with
	imageID_ = iluGenImage();
	if(errorCheck())
	{
		return false;
	}
	// Bind image name so that DevIL performs all subsequent operations on this image
	ilBindImage(imageID_);
	if(errorCheck())
	{
		return false;
	}
	if(!loadImage((ILstring)fileName.c_str()))
	{
		// Use the default image as a place holder
		imageID_ = 0;
		if(errorCheck())
		{
			return false;
		}
		ilBindImage(0);
		if(errorCheck())
		{
			return false;
		}
	}
	if(errorCheck())
	{
		return false;
	}
	//Note: data pointer deleted when the image is deleted
	ILubyte* data = ilGetData();
	if(errorCheck())
	{
		return false;
	}
	info_ = new ILinfo();
	/*Retrieves information about the current image in an ILinfo struct.
	This is useful when you are repeatedly calling ilGetInteger and is more efficient in this case.*/
	iluGetImageInfo(info_);
	if(errorCheck())
	{
		return false;
	}
	width_ = info_->Width;
	height_ = info_->Height;
	format_ = info_->Format;
	glGenTextures(1, &textureID_);
	glBindTexture(GL_TEXTURE_2D, textureID_);
	/*Note: It has been reported that on some ATI drivers, glGenerateMipmap(GL_TEXTURE_2D) has no effect unless you precede it with a call to glEnable(GL_TEXTURE_2D) in this particular case.
	Once again, to be clear, bind the texture, glEnable, then glGenerateMipmap.
	This is a bug and has been in the ATI drivers for a while.
	Perhaps by the time you read this, it will have been corrected. (glGenerateMipmap doesn't work on ATI as of 2011)*/
	glEnable(GL_TEXTURE_2D);
	//Old API:
	glTexImage2D(GL_TEXTURE_2D, 0, format_, width_, height_, 0, format_, GL_UNSIGNED_BYTE, data);
	//New API:
	//Number of mipmaps to generate
	//GLsizei num_mipmaps = 10;
	//glTexStorage2D(GL_TEXTURE_2D, num_mipmaps, mInfo->Format, mInfo->Width, mInfo->Height);
	//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mInfo->Width, mInfo->Height, mInfo->Format, GL_UNSIGNED_BYTE, data);
	// Generate mipmaps for texture
	glGenerateMipmap(GL_TEXTURE_2D);
	// Enable linear filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Set texture wrapping:
	// Note: default is GL_REPEAT
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// Enable anisotropic filtering, if supported
	if(GLEW_EXT_texture_filter_anisotropic)
	{
		// Get the maximum anisotropy value
		GLfloat largest;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &largest);
		// Enable it
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, largest);
	}
	return true;
}
void Texture::unload()
{
	//Unbind the current image in case the current image is this image
	ilBindImage(NULL);
	errorCheck();
	//Delete the DevIL image
	if(info_ != NULL)
	{
		delete info_;
		info_ = NULL;
	}
	errorCheck();
	ilDeleteImages(1, &imageID_);
	errorCheck();
	//Delete the OpenGL texture
	glDeleteTextures(1, &textureID_);
	errorCheck();
}
bool Texture::errorCheck()
{
	ILenum error;
	bool hasError = false;
	while((error = ilGetError()) != IL_NO_ERROR)
	{
		switch(error)
		{
		case IL_COULD_NOT_OPEN_FILE:
			SDL_Log("DevIL Error \"%d\" on \"%s\": %s", error, fileName_.c_str(), "Could not open the file specified. The file may already be open by another app or may not exist.");
			break;
		case IL_FILE_ALREADY_EXISTS:
			SDL_Log("DevIL Error \"%d\" on \"%s\": %s", error, fileName_.c_str(), "The filename specified already belongs to another file. To overwrite files by default read more at ilEnable function.");
			break;
		case IL_FORMAT_NOT_SUPPORTED:
			SDL_Log("DevIL Error \"%d\" on \"%s\": %s", error, fileName_.c_str(), "The format a function tried to use was not able to be used by that function.");
			break;
		case IL_ILLEGAL_FILE_VALUE:
			SDL_Log("DevIL Error \"%d\" on \"%s\": %s", error, fileName_.c_str(), "An illegal value was found in a file trying to be loaded.");
			break;
		case IL_ILLEGAL_OPERATION:
			SDL_Log("DevIL Error \"%d\" on \"%s\": %s", error, fileName_.c_str(), "The operation attempted is not allowable in the current state.");
			break;
		case IL_INTERNAL_ERROR:
			SDL_Log("DevIL Error \"%d\" on \"%s\": %s", error, fileName_.c_str(), "A serious error/bug has occurred within DevIL.");
			break;
		case IL_INVALID_CONVERSION:
			SDL_Log("DevIL Error \"%d\" on \"%s\": %s", error, fileName_.c_str(), "An invalid conversion attempt was tried.");
			break;
		case IL_INVALID_ENUM:
			SDL_Log("DevIL Error \"%d\" on \"%s\": %s", error, fileName_.c_str(), "An unacceptable enumerated value was passed to a function.");
			break;
		case IL_INVALID_EXTENSION:
			SDL_Log("DevIL Error \"%d\" on \"%s\": %s", error, fileName_.c_str(), "The extension of the specified filename was not correct for the type of image loading function.");
			break;
		case IL_INVALID_FILE_HEADER:
			SDL_Log("DevIL Error \"%d\" on \"%s\": %s", error, fileName_.c_str(), "A file's header was incorrect.");
			break;
		case IL_INVALID_PARAM:
			SDL_Log("DevIL Error \"%d\" on \"%s\": %s", error, fileName_.c_str(), "An invalid parameter was passed to a function, such as a NULL pointer.");
			break;
		case IL_INVALID_VALUE:
			SDL_Log("DevIL Error \"%d\" on \"%s\": %s", error, fileName_.c_str(), "An invalid value was passed to a function or was in a file.");
			break;
		case IL_LIB_JPEG_ERROR:
			SDL_Log("DevIL Error \"%d\" on \"%s\": %s", error, fileName_.c_str(), "An error occurred in the libjpeg library.");
			break;
		case IL_LIB_PNG_ERROR:
			SDL_Log("DevIL Error \"%d\" on \"%s\": %s", error, fileName_.c_str(), "An error occurred in the libpng library.");
			break;
		case IL_OUT_FORMAT_SAME:
			SDL_Log("DevIL Error \"%d\" on \"%s\": %s", error, fileName_.c_str(), "Tried to convert an image from its format to the same format.");
			break;
		case IL_OUT_OF_MEMORY:
			SDL_Log("DevIL Error \"%d\" on \"%s\": %s", error, fileName_.c_str(), "Could not allocate enough memory in an operation.");
			break;
		case IL_STACK_OVERFLOW:
			SDL_Log("DevIL Error \"%d\" on \"%s\": %s", error, fileName_.c_str(), "One of the internal stacks was already filled, and the user tried to add on to the full stack.");
			break;
		case IL_STACK_UNDERFLOW:
			SDL_Log("DevIL Error \"%d\" on \"%s\": %s", error, fileName_.c_str(), "One of the internal stacks was empty, and the user tried to empty the already empty stack.");
			break;
		case ILUT_NOT_SUPPORTED:
			SDL_Log("DevIL Error \"%d\" on \"%s\": %s", error, fileName_.c_str(), "A type is valid but not supported in the current build.");
			break;
		default:
			SDL_Log("DevIL Error \"%d\" on \"%s\": %s", error, fileName_.c_str(), "Unknown Error!");
			break;
		}
		hasError = true;
	}
	return hasError;
}
ILboolean Texture::loadImage(const ILstring fileName)
{
	return ilLoadImage(fileName) && !errorCheck();
}
ILboolean Texture::loadImage(const wchar_t* fileName, ILenum type)
{
	/*Types:
		IL_BMP-Load the image as a Microsoft bitmap (.bmp).
		IL_CUT-Load the image as a .cut image.
		IL_DDS-Load a DirectDraw Surface (.dds) image.
		IL_DOOM-Load the image as a Doom texture.
		IL_DOOM_FLAT-Load the image as a Doom flat (floor).
		IL_GIF-Load a Graphics Interchange Format (.gif) file.
		IL_ICO-Load the image as a Microsoft icon (.ico).
		IL_JASC_PAL-Load the file into the current image's palette as a Paint Shop Pro (Jasc) palette.
		IL_JPG-Load the image as a Jpeg (.jpg).
		IL_LIF-Load an Homeworld image.
		IL_MDL-Load an Half-Life model file (.mdl).
		IL_MNG-Load a Multiple Network Graphics (.mng).
		IL_PCD-Load the image as a .pcd image.
		IL_PCX-Load the image as a PCX.
		IL_PIC-Load the image as a .pic image.
		IL_PIX-Load an Alias | Wavefront .pix file.
		IL_PNG-Load a Portable Network Graphics (.png) image.
		IL_PNM-Load a Portable AnyMap (.pbm, .pgm or .ppm).
		IL_PSD-Load a PhotoShop (.psd) file.
		IL_PSP-Load a Paint Shop Pro file.
		IL_PXR-Load a Pxrar (.pxr) file.
		IL_RAW-Load the image as raw data with a 13-byte header.
		IL_SGI-Load an SGI (.bw, .rgb, .rgba or .sgi).
		IL_TGA-Load a TrueVision Targa.
		IL_TIF-Load a TIFF (.tif or .tiff) image.
		IL_TYPE_UNKNOWN-Tells OpenIL to try to determine the type of image present in FileName, File or Lump.
		IL_WAL-Load a Quake .wal texture.
		IL_XPM-Load an .xpm file.*/
	return ilLoad(type, fileName) && !errorCheck();
}
ILboolean Texture::loadImage(ILHANDLE* file, ILenum type)
{
	return ilLoadF(type, file) && !errorCheck();
}
ILboolean Texture::loadImage(const void* lump, ILenum type, ILuint size)
{
	/*Example:
	FILE* file = fopen("test.tga", "rb");
	fseek(file, 0, SEEK_END);
	ILuint size = ftell(file);
	ILubyte* lump = (ILubyte*)malloc(size);
	fseek(file, 0, SEEK_SET);
	fread(lump, 1, size, file);
	fclose(file);
	ILboolean loaded = ilLoadL(IL_TGA, lump, size);
	free(lump);*/
	return ilLoadL(type, lump, size) && !errorCheck();
}
ILboolean Texture::saveImage(const wchar_t* fileName)
{
	return ilSaveImage(fileName) && !errorCheck();
}
ILboolean Texture::saveImage(const wchar_t* fileName, ILenum type)
{
	/*Types:
		IL_BMP-Save the image as a Microsoft bitmap (.bmp).
		IL_CHEAD-Save a C Header style image.
		IL_JPG-Save the image as a Jpeg (.jpg).
		IL_PNM-Save a Portable AnyMap (.pbm, .pgm or .ppm).
		IL_PSD-Save a PhotoShop (.psd) file.
		IL_SGI-Save an SGI (.bw, .rgb, .rgba or .sgi).
		IL_TGA-Save a TrueVision Targa.
		IL_TYPE_UNKNOWN-Tells OpenIL to try to determine the type of image present in FileName, File or Lump.*/
	return ilSave(type, fileName) && !errorCheck();
}
ILboolean Texture::saveImage(ILHANDLE* file, ILenum type)
{
	return ilSaveF(type, file) && !errorCheck();
}
ILboolean Texture::saveImage(void* lump, ILenum type, ILuint size)
{
	return ilSaveL(type, lump, size) && !errorCheck();
}
ILboolean Texture::applyAlienFilter()
{

	return iluAlienify() && !errorCheck();
}
ILboolean Texture::applyBlurAverageFilter(ILuint iter)
{
	return iluBlurAvg(iter) && !errorCheck();
}
/*ILboolean Texture::ApplyFilter(ILuint iter)
{
		/*Note: Blurs an image, using a Gaussian convolution filter.
		A Gaussian blur looks more natural, as diagonal pixels count as the least, side pixels count in the middle and the center pixel counts the most.
		The filter is applied up to Iter number of times, giving more of a blurring effect the higher Iter is, but beware, as it will be slow.*
		iluBlurGaussian(iter);

		/*Note: Detects the edges in the current image using Prewitt filters.
		This is done by combining two convolution filters, a horizontal pass and a vertical pass on the image, then combining them to generate the appropriate edges.*
		iluEdgeDetectP();

		/*Note: Detects the edges in the current image using Sobel filters.
		This is done by combining two convolution filters, a horizontal pass and a vertical pass on the image, then combining them to generate the appropriate edges.*
		iluEdgeDetectS();

		/*Note: Embosses an image, causing it to have a "relief" feel to it using a convolution filter.
		This makes it mostly gray with raised edges and is a different form of edge detection.*
		iluEmboss();

		//Note: This function gamma corrects an image by applying a scaling factor to all the pixels of the image.Values for the scaling factor parameter above 1.0 brighten the image, while values below 1.0 darken the image.
		iluGammaCorrect(ILfloat Gamma);
		//greater than 1.0, the image is brightened.It uses interpolation so it's slower then iluGammaCorrectScale
		iluGammaCorrectInter(ILfloat Gamma);
		iluGammaCorrectScale(ILfloat Gamma);

		//Note: This function creates a negative of the image, effectively inverting all colors (black becomes white, etc.).
		iluNegative();

		//Note: This function adds mono noise to an image to give it a noisy or even "dirty" look. The only parameter it accepts determines the variance factor -- how far from the original pixel's value the new value should be allowed to variate.
		iluNoisify(ILfloat XDim, ILfloat YDim, ILfloat ZDim);

		//Note: This function pixellizes an image, making it appear blocky.
		iluPixelize(ILuint PixSize);

		//Note: This function "sharpens" the current image, making the edges more noticeable and the whole image "less blurry".
		iluSharpen(ILfloat factor, iter);
}*/
void Texture::createFromSurface(SDL_Surface* surface)
{
	fileName_ = "CreateFromSurface";
	width_ = surface->w;
	height_ = surface->h;
	format_ = GL_RGBA;
	// Generate a GL texture
	glGenTextures(1, &textureID_);
	glBindTexture(GL_TEXTURE_2D, textureID_);
	glTexImage2D(GL_TEXTURE_2D, 0, format_, width_, height_, 0, GL_BGRA, GL_UNSIGNED_BYTE, surface->pixels);
	// Use linear filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
void Texture::createForRendering(int width, int height, unsigned int format)
{
	fileName_ = "CreateForRendering";
	width_ = width;
	height_ = height;
	format_ = format;
	// Create the texture id
	glGenTextures(1, &textureID_);
	glBindTexture(GL_TEXTURE_2D, textureID_);
	// Set the image width/height with null initial data
	glTexImage2D(GL_TEXTURE_2D, 0, format_, width_, height_, 0, GL_RGB, GL_FLOAT, nullptr);
	// For a texture we'll render to, just use nearest neighbor
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}
void Texture::setActive(int index)
{
	//Bind image so that DevIL performs all subsequent operations on this image
	ilBindImage(imageID_);
	errorCheck();
	//Bind texture so that OpenGL performs all subsequent operations on this texture
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, textureID_);
}
void Texture::setFrame(int frame)
{
	if (!ilActiveImage(frame))
	{
		SDL_Log("DevIL failed to goto frame '%d' of image: \"%s\"", frame, fileName_.c_str());
	}
	errorCheck();
}
void Texture::setMipmap(int level)
{
	if (!ilActiveMipmap(level))
	{
		SDL_Log("DevIL failed to goto mipmap level '%d' of image: \"%s\"", level, fileName_.c_str());
	}
	errorCheck();
}