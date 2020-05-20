#pragma once
#include <string>
#include <GL/glew.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#include <SDL/SDL.h>

class Texture
{
public:
	//Default Constructor
	Texture(const std::string fileName);
	//Used to create a texture from an SDL Surface
	Texture(SDL_Surface* surface);
	//Used to create a texture for rendering
	Texture(int width, int height, unsigned int format);
	~Texture();
	//Detects if an error has occurred with DevIL
	void errorCheck();
	//Used to load an image file
	ILboolean load(const ILstring fileName);
	//Used to load an image file but it ignores file extensions and uses "type" instead
	ILboolean load(const ILstring fileName, ILenum type);
	//Used to load an image from a previously opened file
	ILboolean load(ILHANDLE* file, ILenum type);
	//Loads an image from a lump/section of memory
	ILboolean load(const void* lump, ILenum type, ILuint size);
	//Used to save an image file
	ILboolean save(const ILstring fileName);
	///Used to save an image file with extension "type" instead
	ILboolean save(const ILstring fileName, ILenum type);
	//Used to save an image to a previously opened file
	ILboolean save(ILHANDLE* file, ILenum type);
	//Saves an image to a lump/section of memory
	ILboolean save(void* lump, ILenum type, ILuint size);
	/*Applies an "Alien" filter to the image
	Note: Images with people in them turn out the best with this filter*/
	ILboolean applyAlienFilter();
	/*Blurs an image using an averaging convolution filter iter number of times.
	Note: All pixels around a pixel get averaged.
	Note: More blurring the higher iter is, but beware, as it will be slow.*/
	ILboolean applyBlurAverageFilter(ILuint iter);
	/*Sets this texture as the current texture so that DevIL and OpenGL performs all subsequent operations it
	Note: resets animation frame and mipmap level*/
	void setActive();
	void setActive(int index);
	/*Sets the frame in an animation chain (image array)
	Note: resets mipmap level*/
	void setFrame(int frame);
	//Sets the mipmap level for an image (image array)
	void setMipmap(int level);
	std::string fileName()
	{
		return fileName_;
	}
	GLuint textureID()
	{
		return textureID_;
	}
	ILuint imageID()
	{
		return imageID_;
	}
	//Image size of data in bytes
	ILuint getSizeOfData()
	{
		return info_->SizeOfData;
	}
	//Image bytes per pixel(not bits)
	ILubyte getBytesPerPixel()
	{
		return info_->Bpp;
	}
	//Depth - ILuint which contains the image depth
	//Format - ILuint which contains the image format
	//Image height
	ILuint getHeight() const
	{
		return height_;
	}
	//NumLayers - ILenum which contains the image number of layer
	//NumMips - ILenum which contains the number of image mipmaps
	//NumNext - ILenum which contains the number of image following the current
	//Origin - ILenum which contains the image origin
	//PalSize - ILenum which contains the image palette size
	//PalType - ILenum which contains the image palette type
	//Type - ILenum which contains the image type
	//Image width
	ILuint getWidth() const
	{
		return width_;
	}
	bool isLoaded() const
	{
		return loaded_;
	}
private:
	GLuint textureID_;
	ILuint imageID_;
	std::string fileName_;
	ILinfo* info_;
	bool loaded_;
	int width_;
	int height_;
	unsigned int format_;
/*
ilActiveFace(ILuint Number);
ilActiveLayer(ILuint Number);
ilApplyProfile(ILstring InProfile, ILstring OutProfile);
ilBlit(ILuint Source, ILint DestX, ILint DestY, ILint DestZ, ILuint SrcX, ILuint SrcY, ILuint SrcZ, ILuint Width, ILuint Height, ILuint Depth);
ilClampNTSC();
ilClearColour(ILclampf Red, ILclampf Green, ILclampf Blue, ILclampf Alpha);
ilClearImage();
ilCloneCurImage();
ilCompressDXT(ILubyte *Data, ILuint Width, ILuint Height, ILuint Depth, ILenum DXTCFormat, ILuint *DXTCSize);
ilCompressFunc(ILenum Mode);
ilConvertImage(ILenum DestFormat, ILenum DestType);
ilCopyImage(ILuint Src);
ilCopyPixels(ILuint XOff, ILuint YOff, ILuint ZOff, ILuint Width, ILuint Height, ILuint Depth, ILenum Format, ILenum Type, void *Data);
ilCreateSubImage(ILenum Type, ILuint Num);
ilDefaultImage();
ilFormatFunc(ILenum Mode);
ilGetAlpha(ILenum Type);
ilGetLumpPos(void);
ilHint(ILenum Target, ILenum Mode);
ilIsDisabled(ILenum Mode);
ilIsEnabled(ILenum Mode);
ilIsImage(ILuint Image);
ilIsValid(ILenum Type, ILconst_string FileName);
ilIsValidF(ILenum Type, ILHANDLE File);
ilIsValidL(ILenum Type, void *Lump, ILuint Size);
ilKeyColour(ILclampf Red, ILclampf Green, ILclampf Blue, ILclampf Alpha);
ilModAlpha(ILdouble AlphaValue);
ilOriginFunc(ILenum Mode);
ilOverlayImage(ILuint Source, ILint XCoord, ILint YCoord, ILint ZCoord);
ilPopAttrib();
ilPushAttrib(ILuint Bits);
ilRegisterMipNum(ILuint Num);
ilRegisterNumFaces(ILuint Num);
ilRegisterNumImages(ILuint Num);
ilRegisterOrigin(ILenum Origin);
ilRegisterSave(ILconst_string Ext, IL_SAVEPROC Save);
ilRegisterType(ILenum Type);
ilResetRead();
ilResetWrite();
ilSetAlpha(ILdouble AlphaValue);
ilSetDuration(ILuint Duration);
ilSetMemory(mAlloc, mFree);
ilSetPixels(ILint XOff, ILint YOff, ILint ZOff, ILuint Width, ILuint Height, ILuint Depth, ILenum Format, ILenum Type, void *Data);
ilSetRead(fOpenRProc, fCloseRProc, fEofProc, fGetcProc, fReadProc, fSeekRProc, fTellRProc);
ilSetWrite(fOpenWProc, fCloseWProc, fPutcProc, fSeekWProc, fTellWProc, fWriteProc);
ilShutDown();
ilTexImage(ILuint Width, ILuint Height, ILuint Depth, ILubyte NumChannels, ILenum Format, ILenum Type, void *Data);
ilTypeFromExt(ILconst_string FileName);
ilTypeFunc(ILenum Mode);

iluBlurGaussian(ILuint Iter);
iluColoursUsed(void);
iluCompareImage(ILuint Comp);
iluContrast(ILfloat Contrast);
iluCrop(ILuint XOff, ILuint YOff, ILuint ZOff, ILuint Width, ILuint Height, ILuint Depth);
iluEdgeDetectE(void);
iluEdgeDetectP(void);
iluEdgeDetectS(void);
iluEmboss(void);
iluEnlargeCanvas(ILuint Width, ILuint Height, ILuint Depth);
iluEnlargeImage(ILfloat XDim, ILfloat YDim, ILfloat ZDim);
iluEqualize(void);
iluEqualize2(void);
iluConvolution(ILint *matrix, ILint scale, ILint bias);
iluFlipImage(void);
iluGammaCorrect(ILfloat Gamma);
iluInvertAlpha(void);
iluMirror(void);
iluNegative(void);
iluNoisify(ILclampf Tolerance);
iluPixelize(ILuint PixSize);
iluRegionfv(ILpointf *Points, ILuint n);
iluRegioniv(ILpointi *Points, ILuint n);
iluReplaceColour(ILubyte Red, ILubyte Green, ILubyte Blue, ILfloat Tolerance);
iluRotate(ILfloat Angle);
iluSaturate1f(ILfloat Saturation);
iluSaturate4f(ILfloat r, ILfloat g, ILfloat b, ILfloat Saturation);
iluScale(ILuint Width, ILuint Height, 0);
iluScaleAlpha(ILfloat scale);
iluScaleColours(ILfloat r, ILfloat g, ILfloat b);
iluSepia(void);
iluSharpen(ILfloat Factor, ILuint Iter);
iluSwapColours(void);
iluWave(ILfloat Angle);
*/
};