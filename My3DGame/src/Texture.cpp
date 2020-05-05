#include "Texture.h"
#include <locale>
#include <codecvt>
#include <iostream>

/*Texture()
{
	// Creating a blank image to work with
	ILuint ImageName;
	// Generate an image name
	//ILuint ImageName = iluGenImage();
	ilGenImages(1, &ImageName);
	// Bind image name so that DevIL performs all subsequent operations on this image
	ilBindImage(ImageName);
	// Generates power-of-2 mipmaps for an image. Mipmaps are then generated for the image, down to a 1x1 image. To use the mipmaps, see ilActiveMipmap.
	ILboolean built = iluBuildMipmaps();

	/*Creates an ugly 64x64 image of 8x8 black and yellow squares to form a checkerboard pattern.
	Recommended to load this image if an image loading function fails (unless memory could not be allocated).
	This way, the user can easily tell if an image was not loaded.
	Plus, the calling program can continue normally, even though it will have an ugly image.*
	ILboolean ilDefaultImage();

	/*Returns a constant human - readable string describing the current OpenIL implementation.
	Strings:
		IL_VENDOR-Describes the OpenIL vendor(currently Abysmal Software).
		IL_VERSION-String describing the current OpenIL version.Use ilGetInteger with IL_VERSION_NUM to actually check for version differences.*
	const char* ilGetString(ILenum StringName);	
	//Returns a constant human-readable string describing the OpenILU vendor.
	const wchar_t* vendor = iluGetString(ILU_VENDOR);
	//Returns a constant human-readable string of the OpenILU version.
	const wchar_t* versionString = iluGetString(ILU_VERSION_NUM);

	/* Returns the value of a selected mode.
	Modes:
		IL_ACTIVE_IMAGE-Returns the current image number.
		IL_ACTIVE_LAYER-Returns the current layer number.
		IL_ACTIVE_MIPMAP-Returns the current mipmap number.
		IL_CONV_PAL-Returns whether palette'd images are converted to their base palettes types on load - e.g. converted to a bgra image.
		IL_CUR_IMAGE-Returns the current bound image name.
		IL_FILE_MODE-Returns whether file overwriting when saving is enabled.
		IL_FORMAT_MODE-Returns the format images are converted to upon loading.
		IL_FORMAT_SET-Returns whether all images loaded are converted to a specific format.
		IL_IMAGE_BITS_PER_PIXEL-Returns the bits per pixel of the current image's data.
		IL_IMAGE_BYTES_PER_PIXEL-Returns the bytes per pixel of the current image's data.
		IL_IMAGE_FORMAT-Returns the current image's format.
		IL_IMAGE_HEIGHT-Returns the current image's height.
		IL_IMAGE_TYPE-Returns the current image's type.
		IL_IMAGE_WIDTH-Returns the current image's width.
		IL_NUM_IMAGES-Returns the number of images in the current image animation chain.
		IL_NUM_MIPMAPS-Returns the number of mipmaps of the current image.
		IL_ORIGIN_MODE-Returns the current origin position.
		IL_ORIGIN_SET-Returns whether all images loaded and saved adhere to a specific origin.
		IL_PALETTE_BPP-Returns the bytes per pixel of the current image's palette.
		IL_PALETTE_NUM_COLS-Returns the number of colors of the current image's palette.
		IL_PALETTE_TYPE-Returns the palette type of the current image.
		IL_TYPE_MODE-Returns the type images are converted to upon loading.
		IL_TYPE_SET-Returns whether all images loaded are converted to a specific type.
		IL_USE_KEY_COLOUR-Returns whether OpenIL uses a key color(not used yet).
		IL_VERSION_NUM-Returns the version number of the shared library.This can be checked against the IL_VERSION #define.*
	ILboolean ilGetBoolean(ILenum Mode);
	// Returns the mode value in the Param parameter.
	ilGetBooleanv(ILenum Mode, ILboolean* Param);

	/* Sets the value of a selected mode.
	Mode:
		IL_BMP_RLE-Tells DevIL to save.bmp files with RLE - compressed data.
		IL_JPG_QUALITY-Sets the quality.jpg files are saved at.Valid values are in the 0 - 99 range, with 99 being the best quality.
		IL_PNG_INTERLACE-Tells DevIL to save.png files interlaced.
		IL_SGI_RLE-Tells DevIL to save.sgi files with RLE - compressed data.
		IL_TGA_CREATE_STAMP-Sets whether.tga files are saved with a 'postage stamp' in their extension fields(IL_TRUE or IL_FALSE accepted).
		IL_TGA_RLE-Tells DevIL to save.tga files with RLE - compressed data.*
	ilSetInteger(ILenum Mode, ILint Param);

	// Getting an image's info manuely
	mWidth = ilGetInteger(IL_IMAGE_WIDTH);
	mHeight = ilGetInteger(IL_IMAGE_HEIGHT);
	ILuint bpp = ilGetInteger(IL_IMAGE_BPP);
	//ect...
	// Alt function that returns the mode value in the Param parameter.
	ILint* depth;
	iluGetIntegerv(IL_IMAGE_DEPTH, depth);
	/*Retrieves information about the current image in an ILinfo struct.
	This is useful when you are repeatedly calling ilGetInteger and is more efficient in this case.*
	ILinfo* info;
	iluGetImageInfo(info);
	/*Data in ILinfo struct:
		SizeOfData-ILuint which contains the image size of data in bytes
		Bpp-ILubyte which contains the image Bytes per pixel (not bits)
		Depth-ILuint which contains the image depth
		Format-ILuint which contains the image format
		Height-ILuint which contains the image height
		NumLayers-ILenum which contains the image number of layer
		NumMips-ILenum which contains the number of image mipmaps
		NumNext-ILenum which contains the number of image following the current
		Origin-ILenum which contains the image origin
		PalSize-ILenum which contains the image palette size
		PalType-ILenum which contains the image palette type
		Type-ILenum which contains the image type
		Width-ILuint which contains the image width*

	// Creates a copy of the image data, quicksorts it and counts the number of unique colors in the image. This value is returned without affecting the original image.
	ILboolean iluColoursUsed();

	// Compares the current image to the image having the name in Comp.If both images are identical, IL_TRUE is returned.IL_FALSE is returned if the images are not identical.The bound image before calling this function remains the bound image after calling ilCompareImage.
	ILboolean iluCompareImage(ILuint Comp);

	// Changes the contrast of an image by using interpolation and extrapolation.Common values for Contrast are in the range - 0.5 to 1.7.Anything below 0.0 generates a negative of the image with varying contrast. 1.0 outputs the original image. 0.0 - 1.0 lowers the contrast of the image. 1.0 - 1.7 increases the contrast of the image.This effect is caused by interpolating(or extrapolating) the source image with a totally grey image.
	ILboolean iluContrast(ILfloat Contrast);

	/*Crops the current image to new dimensions.
	The new image appears the same as the original, but portions of the image are clipped - off, depending on the values of the parameters of these functions.
	If XOff + Width, YOff + Height or ZOff + Depth is larger than the current image's dimensions, ILU_ILLEGAL_OPERATION is set. 
	If ZOff is minus or equal to one the crop will be done only on 2 dimensions
	Parameters:
		XOff-Number of pixels to skip in the x direction.
		YOff-Number of pixels to skip in the y direction.
		ZOff-Number of pixels to skip in the z direction.
		Width-Number of pixels to preserve in the x direction.
		Height-Number of pixels to preserve in the y direction.
		Depth-Number of pixels to preserve in the z direction.*
	ILboolean iluCrop(ILuint XOff, ILuint YOff, ILuint ZOff, ILuint Width, ILuint Height, ILuint Depth);

	/* Sets the current clearing color to be used by future calls to ilClearImage.
	iluRotate and iluEnlargeCanvas both use these values to clear blank space in images, too.
	Parameters:
		Red-Amount of red to clear to.
		Green-Amount of green to clear to.
		Blue-Amount of blue to clear to.
		Alpha-Amount of alpha to clear to.*/
	/* Clears the image to the colors specified in ilClearColour.
	If the current image is of format IL_COLOR_INDEX, the image is cleared to all zeros, and the palette is changed to one entry of all zeros.
	If the current image is of format IL_LUMINANCE, the image is cleared to all zeros.*
	ILboolean ilClearImage();
	// Rotates an image about the center by Angle degrees.The background where there is space left by the rotation will be set to the clear color.
	ILboolean iluRotate(ILfloat Angle);
	// Enlarges the canvas of the current image, clearing the background to the color specified in ilClearColour. To control the placement of the image, use iluImageParameter
	ILboolean iluEnlargeCanvas(ILuint Width, ILuint Height, ILuint Depth);

	// Inverts an image over the x axis.The image will be upside - down after calling this function.If this function is called twice in succession, the image is restored to its original state.A version of this function in OpenIL is used throughout internally when loading images to correct images that would otherwise be upside - down.Using ilOriginFunc will essentially tell the library which way is up.
	ILboolean iluFlipImage();

	/* Adjust the gamma of the current image.
	If Gamma < 1.0, the image is darkened.
	If Gamma > 1.0, the image is brightened.
	Is a slower function of iluGammaCorrectScale and uses interpolation to achieve its effect.*
	ILboolean iluGammaCorrectInter(ILfloat Gamma);
	/* Adjust the gamma of the current image.
	If Gamma < 1.0, the image is darkened.
	If Gamma > 1.0, the image is brightened.
	Achieves its effect by scaling the colors via iluScaleColours.*
	ILboolean iluGammaCorrectScale(ILfloat Gamma);

	/* Modifies the behavior of some ilu functions.
	Right now, it controls the behavior of iluEnlargeCanvasand iluScale.
	For ILU_FILTER, values for Param other than ILU_NEAREST, ILU_LINEAR and ILU_BILINEAR are higher - quality scaling filters and take longer to perform.*
	ILvoid iluImageParameter(ILenum PName, ILenum Param);
	/*PName-Parameter name:
		ILU_FILTER-Which filter iluScale uses, default is ILU_NEAREST
		ILU_PLACEMENT-Where to place the image when using iluEnlargeCanvas.The default is ILU_CENTER
	Param-Behavior to use:
	Scale Filters:
		ILU_BILINEAR-Uses a bilinear(or trilinear for 3d images) interpolation filter to scale the image(looks best)
		ILU_LINEAR-Uses a linear interpolation filter to scale the image(looks better)
		ILU_NEAREST-Uses a nearest filter to scale the image(looks worst)
		ILU_SCALE_BELL-Uses a bell filter to scale the image
		ILU_SCALE_BOX-Uses a box filter to scale the image
		ILU_SCALE_BSPLINE-Uses a b-spline filter to scale the image
		ILU_SCALE_LANCZOS3-Uses a Lanczos filter to scale the image
		ILU_SCALE_MITCHELL-Uses a Mitchell filter to scale the image
		ILU_SCALE_TRIANGLE-Uses a triangle filter to scale the image
	Placements:
		ILU_CENTER-Places the image in the center of the enlarged canvas
		ILU_LOWER_LEFT-Places the image in the lower left of the enlarged canvas
		ILU_LOWER_RIGHT-Places the image in the lower right of the enlarged canvas
		ILU_UPPER_LEFT-Places the image in the upper left of the enlarged canvas
		ILU_UPPER_RIGHT-Places the image in the upper right of the enlarged canvas*

	// Inverts the alpha of the currently bound image.
	ILboolean iluInvertAlpha();

	// Mirrors an image across its y axis, making it appear backwards.
	ILboolean iluMirror();

	// Creates a negative version of an image, like it was viewed as a picture negative instead of the actual picture.The effect is caused by inverting the image's colours, such as a green pixel would become purple (red-blue).
	ILboolean iluNegative();

	// Enlarges an image's dimensions by multipliers, via iluScale. This function could be useful if you wanted to double the size of all images or something similar
	ILboolean iluNoisify(ILfloat XDim, ILfloat YDim, ILfloat ZDim);

	/*Performs the effect that can be seen on television, where people want their identity to remain anonymous, so the editors cover the person's face with a very blocky pixelized version.
	PixSize specifies how blocky the image should be, with 1 being the lowest blockiness (doesn't change the image).*
	ILboolean iluPixelize(ILuint PixSize);

	/* Applies a saturation consistent with the IL_LUMINANCE conversion values to red, green and blue.
	Amount of saturation to apply to the current bound image.the value must go from - 1.0 to 1.0*
	ILboolean iluSaturate1f(ILfloat Saturation);
	//iluSaturate4f(No description given but likely similar to iluSaturate1f and/or iluScaleColours)
	ILboolean iluSaturate4f(ILfloat r, ILfloat g, ILfloat b);

	/* Scales the individual color components of the current bound image.
	Using 1.0f as any of the parameters yields that color component's original plane in the new image.
	r-Amount of red to use from the original image
	g-Amount of green to use from the original image
	b-Amount of blue to use from the original image*
	ILboolean iluScaleColours(ILfloat r, ILfloat g, ILfloat b);

	/* Scales the image to the new dimensions specified, shrinking or enlarging the image, depending on the image's original dimensions.
	There are different filters that can be used to scale an image, and which filter to use can be specified via iluImageParameter.*
	ILboolean iluScale(ILuint Width, ILuint Height, ILuint Depth);

	/* Either sharpen or blur an image, depending on the value of Factor.
	iluBlurAvgand iluBlurGaussian are much faster for blurring, though.
	When Factor is 1.0, the image goes unchanged.
	When Factor is in the range 0.0 - 1.0, the current image is blurred.
	When Factor is in the range 1.0 - 2.5, the current image is sharpened.
	To achieve a more pronounced sharpening / blurring effect, simply increase the number of iterations by increasing the value passed in Iter.*
	ILboolean iluSharpen(ILfloat Factor, ILuint Iter);

	/* Swaps the color order of the current image.
	If the current image is in bgr(a) format, iluSwapColours will change the image to use rgb(a) format, or vice - versa.
	This can be helpful when you want to manipulate the image data yourself but only want to use a certain colour order.
	To determine the current color order, call ilGetInteger with the IL_IMAGE_FORMAT parameter.*
	ILboolean iluSwapColours();

	/*modifies behavior, in order to optimize either speed, memory, compression or quality, depending wholly on what the user desires.
	Targets:An enum indicating what behavior of the library is to be controlled.
		IL_COMPRESSION_HINT-Controls whether compression is used when saving images.
		IL_MEM_SPEED_HINT-Controls the memory used vs.speed trade off.
	Modes:The desired behavior.
		IL_DONT_CARE-The client does not have a preference.
		IL_FASTEST-Makes the target use a faster but more memory - intensive algorithm.
		IL_LESS_MEM-Makes the target use less memory but a potentially slower algorithm.
		IL_NO_COMPRESSION-Specifies that OpenIL should never use compression when saving.
		IL_USE_COMPRESSION-Specifies that OpenIL should use compression when saving, if possible.*
	ilHint(ILenum Target, ILenum Mode);

	/* Sets the origin to be used when loading all images, so that any image with a different origin will be flipped to have the set origin.
	This behavior is actually disabled by default but can be enabled using ilEnable with the IL_ORIGIN_SET parameter.
	Modes:
		IL_ORIGIN_LOWER_LEFT-The origin is in the lower left of the image.
		IL_ORIGIN_UPPER_LEFT-The origin is in the upper left of the image.*
	ILboolean ilOriginFunc(ILenum Mode);

	/* Copies the image named by Src onto the current bound image.XCoord, YCoordand ZCoord are allowed to be any number, even negative numbers, for if you want to start copying Src in the middle of it to the current image's left side.
	If the image named by Src has alpha components, then blending will occur, instead of just a simple overlay.*
	ILboolean ilOverlayImage(ILuint Src, ILuint XCoord, ILuint YCoord, ILuint ZCoord);

	// Allows you to set how long to show the currently bound image.This function can also change the durations of individual images in animation chains.
	ILboolean ilSetDuration(ILuint Duration);

	// sets the current image to be an image in an animation chain (Number-Animation number to select as current)
	ILboolean ilActiveImage(ILuint Number);

	/* Sets a mipmap of the image as the current mipmap.
	Currently, the only way to generate mipmaps is by calling iluBuildMipmaps.
	If neither function has been called for the current image, no mipmaps exist for it.
	If Number is 0, then the current base image is set.
	Number-Mipmap level to select as current.*
	ILboolean ilActiveMipmap(ILuint Number);

	/*Creates a copy of the current image and returns the id of the new image.
	If a sub-image of the current image is currently selected via ilActiveImage, ilActiveLayer or ilActiveMipmap, the sub-image is copied, not the base image.*
	ILuint ilCloneCurImage();

	/* Applies a color profile (files with extension .icm) to the currently bound image.
	InProfile describes the current image's color space, and OutProfile describes the color space to convert the currently bound image to.
	If InProfile is NULL, DevIL attempts to use the color profile present in the image, if one is present, else it returns IL_FALSE.
	InProfile-Profile file describing the color space the image is in.
	OutProfile-Profile file describing the color space to convert the image to.*
	ILboolean ilApplyProfile(const ILstring InProfile, const ILstring OutProfile );

	/*ilConvertImage converts the current bound image from its format/type to DestFormat and DestType.
	Almost all conversions are allowable.
	DestFormat-The format the current image should be converted to.
	DestType-The type the current image should be converted to.
	Formats:
		IL_BGR
		IL_BGRA
		IL_COLOUR_INDEX
		IL_LUMINANCE
		IL_LUMINANCE_ALPHA
		IL_RGB
		IL_RGBA
	Types:	
		IL_BYTE
		IL_DOUBLE
		IL_FLOAT
		IL_INT
		IL_SHORT
		IL_UNSIGNED_BYTE
		IL_UNSIGNED_INT
		IL_UNSIGNED_SHORT*
	ILboolean ilConvertImage(ILenum DestFormat, ILenum DestType);

	//Copies the attributes and data from the image named in Src. The same image bound before calling ilCopyImage remains bound afterward.
	ILboolean ilCopyImage(ILuint Src);

	/*Copies a block of pixels from the Data buffer to the current image's data.
	XOff, YOff and ZOff can be used to skip a certain number of pixels in each respective direction.
	If XOff+Width, YOff+Height and/or ZOff+Depth > current image's dimensions, only the current image's width, height and/or depth number of pixels will be copied.
	XOff-Where to begin copying pixels from in the x direction.
	YOff-Where to begin copying pixels from in the y direction.
	ZOff-Where to begin copying pixels from in the z direction.
	Width-How many pixels to copy in the x direction.
	Height-How many pixels to copy in the y direction.
	Depth- How many pixels to copy in the z direction.
	Format-The desired format the output should be.
	Type-The desired type the output should be.
	Data-User-defined buffer to copy the image data to.*
	ilCopyPixels(ILuint XOff, ILuint YOff, ILuint ZOff, ILuint Width, ILuint Height, ILuint Depth, ILenum Format, ILenum Type, ILvoid* Data);

	/*ilSetPixels (No description given but likely similar to ilCopyPixels)
	XOff-Where to begin copying pixels from in the x direction.
	YOff-Where to begin copying pixels from in the y direction.
	ZOff-Where to begin copying pixels from in the z direction.
	Width-How many pixels to copy in the x direction.
	Height-How many pixels to copy in the y direction.
	Depth- How many pixels to copy in the z direction.
	Format-The desired format the output should be.
	Type-The desired type the output should be.
	Data-User-defined buffer to copy the image data to.*
	ilSetPixels(ILuint XOff, ILuint YOff, ILuint ZOff, ILuint Width, ILuint Height, ILuint Depth, ILenum Format, ILenum Type, ILvoid* Data);

	/*Any current image data is destroyed by ilTexImage and replaced by a new image with the attributes specified.
	The new image data has undefined values.
	To set the new image data to a certain value, use ilClearImage or ilClearImageTo.
	Width-Specifies the new image width.
		This cannot be 0.
	Height-Specifies the new image height.
		This cannot be 0.
	Depth-Specifies the new image depth.
		Anything greater than 1 will make the image 3d.
		This cannot be 0.
	numberOfChannels-Specifies the new channels.
		Common values are 3 and 4.
	Format-Specifies the data format this image has.
		For a list of values this can be, see the See Also section.
	Type-Specifies the data format this image has.
		For a list of values this can be, see the See Also section.
	Data-Specifies data that should be copied to the new image.
		If this parameter is NULL, no data is copied, and the new image data consists of undefined values.*
	ILboolean ilTexImage(ILuint Width, ILuint Height, ILuint Depth, ILubyte numberOfChannels, ILenum Format, ILenum Type, ILvoid* Data);

	/* Gives DevIL users the option to set strings in certain file formats that have fields for strings, making DevIL highly customizable.
	Choose one of the acceptable parameters for Mode and specify any string you want.
	If the string is too long, it will be truncated when writing to the file.
	String-String to use for setting a string field of a specified image format.
	Mode: Specifies the string to be set.
		IL_PNG_AUTHNAME_STRING
		IL_PNG_DESCRIPTION_STRING
		IL_PNG_TITLE_STRING
		IL_TGA_AUTHCOMMENT_STRING
		IL_TGA_AUTHNAME_STRING
		IL_TGA_ID_STRING
		IL_TIF_AUTHNAME_STRING
		IL_TIF_DESCRIPTION_STRING
		IL_TIF_DOCUMENTNAME_STRING
		IL_TIF_HOSTCOMPUTER_STRING*
	ilSetString(char* String, ILenum Mode);

	/*Is not yet used according to old documentation
	Number-Layer number to select as current*
	ILboolean ilActiveLayer(ILuint Number);

	/* Copies the bits specified and pushes them onto the stack.
	Bits:
		IL_ALL_ATTRIB_BITS-Preserves all OpenIL states and attributes.
		IL_FILE_BIT-Preserves whether OpenIL is allowed to overwrite files when saving(set by ilEnable, ilDisable).
		IL_ORIGIN_BIT-Preserves the origin state set by ilOriginFunc.
		IL_PAL_BIT-Preserves whether OpenIL is supposed to convert palette'd to truecolour images (set by ilEnable, ilDisable).*
	ilPushAttrib(ILuint Bits);
	// Pops the last pushed stack entry off the stack and copies the bits specified when pushed by ilPushAttrib to the previous set of states.
	ilPopAttrib();

	// Getting/Setting the image's data:
	//Note: data pointer deleted when the image is deleted
	ILubyte* Data = ilGetData();
	// Updates the current bound image data(bound by ilBindImage) with new data of the same size.This way new memory does not have to be allocated, so transfers are much faster.
	ILboolean ilSetData(const void* Data);

	/* Returns whether the image name in Image is a valid image in use.
	If the image name in Image is in use, ilIsImage returns IL_TRUE.
	If Image is 0, ilIsImage returns IL_FALSE, because the default image is a special imageand is never returned by ilGenImages.
	If the image name has been deleted by ilDeleteImages or never generated byilGenImages, IL_FALSE is returned.*
	ILboolean ilIsImage(ILuint Image);

	// Getting an OpenGL texture
	GLuint texture;
	texture = ilutGLBindTexImage();

	// Deleting an image
	// Unbind the current image incase the current image is this image
	ilBindImage(NULL);
	ilDeleteImages(1, &ImageName);
	//iluDeleteImage(ImageName);

}*/
/*~Texture()
{
	// OpenGL specific stuff (ILUT):
	//ilutPushAttrib(No description given but likely similar to ilPushAttrib)
	ilutPushAttrib(ILuint Bits);
	//Pops the last pushed stack entry off the stack and copies the bits specified when pushed by ilutPushAttrib to the previous set of states.
	ilutPopAttrib();

	/* Returns whether the mode indicated by Mode is enabled.
	Modes:
		ILUT_OPENGL_CONV-Whether ilut is allowed to use GL_RGB8and GL_RGBA8 instead of GL_RGB8 or GL_RGBA8(useful with nVidia cards).
		ILUT_PALETTE_MODE-Not used right now.*
	ILboolean ilutIsEnabled(ILenum Mode);
	// Opposite of ilutIsEnabled
	ILboolean ilutIsDisabled(ILenum Mode);

	/* Returns the value of a selected mode.
	Mode:
		ILUT_OPENGL_CONV-An ILUT mode used to track if OpenIL is allowed to use GL_RGB8and GL_RGBA8 instead of GL_RGB8 or GL_RGBA8(useful with nVidia cards)
		ILUT_PALETTE_MODE-An ILUT mode not used right now
		ILUT_VERSION_NUM-Returns the version number of the shared library.This can be checked against the ILUT_VERSION #define*
	ILint ilutGetInteger(ILenum mode);

	/* Returns the value of a selected mode.
	Modes:
		ILUT_OPENGL_CONV-An ILUT mode used to track if OpenIL is allowed to use GL_RGB8and GL_RGBA8 instead of GL_RGB8 or GL_RGBA8(useful with nVidia cards)
		ILUT_PALETTE_MODE-An ILUT mode not used right now*
	ILboolean ilutGetBoolean(ILenum mode);
	// Returns the value of a selected mode via a pointer
	ilutGetBooleanv(ILenum mode, ILboolean * Param);

	/*Binds an image to an OpenGL texture and creates mipmaps, generates a texture via glGenTextures, binds the current OpenIL image to it, generates mipmaps via gluBuild2DMipmaps and returns the texture name.
	This function is a more general purpose version of ilutOglBindTexImage.*
	GLuint ilutGLBindMipmaps();

	// Loads an image directly to an OpenGL texture, skipping the use of OpenIL image names.
	GLuint ilutGLLoadImage(char* filename);

	/* The mipmapping equivalent of ilutOglBindTexImage.
	This function is similar to ilutOglBuildMipmaps in that it performs the same operations, but it generates and binds an OpenGL texture and sets various texture states before generating mipmaps.
	The return value is the OpenGL texture name.
	If it is 0, an error occurred.*
	GLuint mTextureID = ilutGLBindTexImage();

	/*Binds an image to an OpenGL texture, simply calls glTexImage2D with the current bound image's data and attributes.
	Level-Texture level to place the image at. 0 is the base image level, and anything lower is a mipmap.
	Use ilActiveMipmap to access OpenIL's mipmaps.*
	ILboolean ilutGLTexImage(GLuint Level);

	/* Generates mipmaps via gluBuild2DMipmaps from an image.
	This function is similar to ilutGLTexImage but creates mipmaps.*
	ILboolean ilutGLBuildMipmaps();

	/* All this function does is call glTexImage2D after performing any required conversions.
	The Level parameter allows you to manually set mipmaps.*
	ILboolean ilutOglTexImage(GLuint Level);

	/* Similar to the aforementioned ilutOglTexImage, except it generates and binds a texture via glGenTexturesand glBindTexture.
	When you call ilutOglTexImage, you already have to have your texture bound, so this function makes it easier for the developer.
	Also, this function sets many of the texture environment settings, so this integrated solution may not be perfect for you.
	The return value is the OpenGL texture name.
	If it is 0, an error occurred.*
	GLuint ilutOglBindTexImage();

	/* The mipmapping equivalent of ilutOglTexImage.
	It creates mipmaps via gluBuild2DMipmaps.*
	ILboolean ilutOglBuildMipmaps();

	/* Feel that there is no reason to mess directly with DevIL, as you will not need a local copy of the DevIL images you load as OpenGL textures?
	If so, ilutOglLoadImage is for you.This function bypasses the need to generate image names, etc.
	Just call ilutOglLoadImage with the name of the file you wish to load, and it will load the image directly into an OpenGL texture, then delete all unnecessary local data.
	This function is quick and simple to use if it is all you will need.*
	GLuint ilutOglLoadImage(char* FileName);
}*/
Texture::Texture():mFileName(""), mTextureID(0), mImageID(0), mInfo(nullptr), mWidth(0), mHeight(0)
{
	//
}
Texture::~Texture()
{
	//
}
bool Texture::Load(const std::string& fileName)
{
	mFileName = fileName;
	// Creating a blank image to work with
	mImageID = iluGenImage();	
	if(ErrorCheck())
	{
		return false;
	}
	// Bind image name so that DevIL performs all subsequent operations on this image
	ilBindImage(mImageID);
	if(ErrorCheck())
	{
		return false;
	}
	std::wstring wstr = StringToWstring(fileName);
	bool loaded = LoadData(wstr.c_str());
	if (!loaded)
	{
		// Use the default image as a place holder
		mImageID = 0;
		if(ErrorCheck())
		{
			return false;
		}
		ilBindImage(0);
		if(ErrorCheck())
		{
			return false;
		}
	}	
	if(ErrorCheck())
	{
		return false;
	}
	//Note: data pointer deleted when the image is deleted
	ILubyte* data = ilGetData();
	if (ErrorCheck())
	{
		return false;
	}
	mInfo = new ILinfo();
	/*Retrieves information about the current image in an ILinfo struct.
	This is useful when you are repeatedly calling ilGetInteger and is more efficient in this case.*/
	iluGetImageInfo(mInfo);
	if(ErrorCheck())
	{
		return false;
	}
	/*Data in ILinfo struct:
		SizeOfData-ILuint which contains the image size of data in bytes
		Bpp-ILubyte which contains the image Bytes per pixel (not bits)
		Depth-ILuint which contains the image depth
		Format-ILuint which contains the image format
		Height-ILuint which contains the image height
		NumLayers-ILenum which contains the image number of layer
		NumMips-ILenum which contains the number of image mipmaps
		NumNext-ILenum which contains the number of image following the current
		Origin-ILenum which contains the image origin
		PalSize-ILenum which contains the image palette size
		PalType-ILenum which contains the image palette type
		Type-ILenum which contains the image type
		Width-ILuint which contains the image width*/
	mWidth = mInfo->Width;
	mHeight = mInfo->Height;
	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);
	/*Note: It has been reported that on some ATI drivers, glGenerateMipmap(GL_TEXTURE_2D) has no effect unless you precede it with a call to glEnable(GL_TEXTURE_2D) in this particular case.
	Once again, to be clear, bind the texture, glEnable, then glGenerateMipmap.
	This is a bug and has been in the ATI drivers for a while.
	Perhaps by the time you read this, it will have been corrected. (glGenerateMipmap doesn't work on ATI as of 2011)*/
	glEnable(GL_TEXTURE_2D);
	//Old API:
	glTexImage2D(GL_TEXTURE_2D, 0, mInfo->Format, mInfo->Width, mInfo->Height, 0, mInfo->Format, GL_UNSIGNED_BYTE, data);
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
void Texture::Unload()
{
	// Unbind the current image in case the current image is this image
	// Note: 0 is the default image
	ilBindImage(0);
	ErrorCheck();
	// Delete the DevIL image
	iluDeleteImage(mImageID);
	ErrorCheck();
	// Delete the OpenGL texture
	glDeleteTextures(1, &mTextureID);
	// May not be necessary
	delete mInfo;
	mInfo = nullptr;
}
bool Texture::ErrorCheck()
{
	//Detecting an error.
	ILenum error;
	while ((error = ilGetError()) != IL_NO_ERROR)
	{
		SDL_Log("DevIL Error \"%d\": %s", error, WstringToString(iluErrorString(error)).c_str());
		return true;
	}
	return false;
	/*Errors:
		IL_COULD_NOT_OPEN_FILE-Could not open the file specified.The file may already be open by another app or may not exist.
		IL_FILE_ALREADY_EXISTS-The filename specified already belongs to another file.To overwrite files by default read more at ilEnable function.
		IL_FORMAT_NOT_SUPPORTED-The format a function tried to use was not able to be used by that function.
		IL_ILLEGAL_FILE_VALUE-An illegal value was found in a file trying to be loaded.
		IL_ILLEGAL_OPERATION-The operation attempted is not allowable in the current state.The function returns with no ill side effects.
		IL_INTERNAL_ERROR-A serious error has occurred.Please e - mail an admin with the conditions leading up to this error being reported.
		IL_INVALID_CONVERSION-An invalid conversion attempt was tried.
		IL_INVALID_ENUM-An unacceptable enumerated value was passed to a function.
		IL_INVALID_EXTENSION-The extension of the specified filename was not correct for the type of image - loading function.
		IL_INVALID_FILE_HEADER-A file's header was incorrect.
		IL_INVALID_PARAM-An invalid parameter was passed to a function, such as a NULL pointer.
		IL_INVALID_VALUE-An invalid value was passed to a function or was in a file.
		IL_LIB_JPEG_ERROR-An error occurred in the libjpeg library.
		IL_LIB_PNG_ERROR-An error occurred in the libpng library.
		IL_NO_ERROR-No detectable error has occurred.
		IL_OUT_FORMAT_SAME-Tried to convert an image from its format to the same format.
		IL_OUT_OF_MEMORY-Could not allocate enough memory in an operation.
		IL_STACK_OVERFLOW-One of the internal stacks was already filled, and the user tried to add on to the full stack.
		IL_STACK_UNDERFLOW-One of the internal stacks was empty, and the user tried to empty the already empty stack.
		IL_UNKNOWN_ERROR-No function sets this yet, but it is possible(not probable) it may be used in the future.
		ILU_INTERNAL_ERROR-A serious error/bug has occurred within DevIL.
		ILU_INVALID_ENUM-A Parameter was invalid or out of range
		ILU_INVALID_PARAM-A Parameter was invalid or out of range
		ILU_INVALID_VALUE-An invalid value was passed to a function or was in a file
		ILU_OUT_OF_MEMORY-Could not allocate enough memory in an operation
		ILU_ILLEGAL_OPERATION-The operation attempted is not allowable in the current state.
			The function returns with no ill side effects.
			Generally there is currently no image bound or it has been deleted via ilDeleteImages.
			You should use ilGenImages and ilBindImage before calling the function.
		ILUT_COULD_NOT_OPEN_FILE-Could not open the file specified.The file may already be open by another app or may not exist.
		ILUT_ILLEGAL_OPERATION-The operation attempted is not allowable in the current state.The function returns with no ill side effects.
		ILUT_INVALID_ENUM-An unacceptable enumerated value was passed to a function.
		ILUT_INVALID_PARAM-An invalid parameter was passed to a function, such as a NULL pointer.
		ILUT_INVALID_VALUE-An invalid value was passed to a function or was in a file.
		ILUT_NOT_SUPPORTED-A type is valid but not supported in the current build.
		ILUT_OUT_OF_MEMORY-Could not allocate enough memory in an operation.
		ILUT_STACK_OVERFLOW-One of the internal stacks was already filled, and the user tried to add on to the full stack.
		ILUT_STACK_UNDERFLOW-One of the internal stacks was empty, and the user tried to empty the already empty stack.
	*/
}
std::wstring Texture::StringToWstring(const std::string& str)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.from_bytes(str);
}
std::string Texture::WstringToString(const std::wstring& wstr)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(wstr);
}
bool Texture::LoadData(const wchar_t* fileName)
{
	/*General interface to the specific internal file-loading routines.
	The approach ilLoadImage takes toward determining image types is three-pronged.
	First, it finds the extension and checks to see if any user-registered functions (registered through ilRegisterLoad) match the extension.
	If nothing matches, it takes the extension and determines which function to call based on it.
	Lastly, it attempts to identify the image based on various image header verification functions, such as ilIsValidPngF.
	If all this checking fails, IL_FALSE is returned with no modification to the current bound image.*/
	return ilLoadImage(fileName);
}
bool Texture::LoadData(const wchar_t* fileName, ILenum type)
{
	/*Can be used much in the same way ilLoadImage is used, except with ilLoad, it is possible to force OpenIL to load a file as a specific image format, no matter what the extension.
	Types:
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
	return ilLoad(type, fileName);
}
bool Texture::LoadData(ILHANDLE* file, ILenum type)
{

	// Loads an image from a previously opened file
	return ilLoadF(type, file);
}
bool Texture::LoadData(const void* lump, ILenum type, ILuint size)
{
	//Loads an image from a memory lump
	return ilLoadL(type, lump, size);
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
}
bool Texture::SaveData(const wchar_t* fileName)
{
	return ilSaveImage(fileName);
}
bool Texture::SaveData(const wchar_t* fileName, ILenum type)
{
	/* Can be used much in the same way ilSaveImage is used, except with ilSave, it is possible to force OpenIL to save a file as a specific image format, no matter what the extension.
	Types:
		IL_BMP-Save the image as a Microsoft bitmap (.bmp).
		IL_CHEAD-Save a C Header style image.
		IL_JPG-Save the image as a Jpeg (.jpg).
		IL_PNM-Save a Portable AnyMap (.pbm, .pgm or .ppm).
		IL_PSD-Save a PhotoShop (.psd) file.
		IL_SGI-Save an SGI (.bw, .rgb, .rgba or .sgi).
		IL_TGA-Save a TrueVision Targa.
		IL_TYPE_UNKNOWN-Tells OpenIL to try to determine the type of image present in FileName, File or Lump.*/
	return ilSave(type, fileName);
}
bool Texture::SaveData(ILHANDLE* file, ILenum type)
{
	// Saves an image to a previously opened file
	return ilSaveF(type, file);
}
bool Texture::SaveData(void* lump, ILenum type, ILuint size)
{
	// Saves an image to a memory lump
	return ilSaveL(type, lump, size);
}
ILboolean Texture::ApplyFilter(TextureFilter filter)
{
	ILboolean filterApplied = false;
	switch (filter)
	{
	case FILTER_ALIENIFY:
		// Note: Images with people in them turn out the best with this filter, and no, it doesn't just change the image to all green.
		filterApplied = iluAlienify();
		break;
	case FILTER_BIT_1:
		// Switches the upper and lower nibbles of every byte in the image
		//filterApplied = iluBitFilter1();
		break;
	case FILTER_BIT_2:
		// Reverses the order of all bits in an image
		//filterApplied = iluBitFilter2();
		break;
	case FILTER_BIT_3:
		// This filter AND's every byte with a random value from 0 - 255. This image is a type of noise filter
		//filterApplied = iluBitfilter3();
		break;
	case FILTER_BLUR_AVG:
		/* Note: Blurs an image using an averaging convolution filter. 
		All pixels around a pixel get averaged.
		The filter is applied up to Iter number of times, giving more of a blurring effect the higher Iter is, but beware, as it will be slow.*/
		//filterApplied = iluBlurAvg();
		break;
	case FILTER_BLUR_GAUSSIAN:
		/* Note: Blurs an image, using a Gaussian convolution filter.
		A Gaussian blur looks more natural, as diagonal pixels count as the least, side pixels count in the middle and the center pixel counts the most.
		The filter is applied up to Iter number of times, giving more of a blurring effect the higher Iter is, but beware, as it will be slow.*/
		//filterApplied = iluBlurGaussian();
		break;
	case FILTER_EDGE_DETECT_PREWITT:
		/*Note: Detects the edges in the current image using Prewitt filters.
		This is done by combining two convolution filters, a horizontal pass and a vertical pass on the image, then combining them to generate the appropriate edges.*/
		filterApplied = iluEdgeDetectP();
		break;
	case FILTER_EDGE_DETECT_SOBEL:
		/*Note: Detects the edges in the current image using Sobel filters.
		This is done by combining two convolution filters, a horizontal pass and a vertical pass on the image, then combining them to generate the appropriate edges.*/
		filterApplied = iluEdgeDetectS();
		break;
	case FILTER_EMBOSS:
		/* Note: Embosses an image, causing it to have a "relief" feel to it using a convolution filter.
		This makes it mostly gray with raised edges and is a different form of edge detection.*/
		filterApplied = iluEmboss();
		break;
	case FILTER_GAMMA_CORRECT_CURVE:
		//filterApplied = 
		break;
	case FILTER_NEGATIVE:
		//filterApplied = 
		break;
	case FILTER_NOISE:
		//filterApplied = 
		break;
	case FILTER_PIXELIZE:
		//filterApplied = 
		break;
	case FILTER_SHARPEN:
		//filterApplied = 
		break;
	default:
		//
		break;
		/*
		//This function gamma corrects an image by applying a scaling factor to all the pixels of the image.Values for the scaling factor parameter above 1.0 brighten the image, while values below 1.0 darken the image.
		iluGammaCorrectCurve
		//iluNegative creates a negative of the image, effectively inverting all colors (black becomes white, etc.).
		iluNegative
		//iluNoise adds mono noise to an image to give it a noisy or even "dirty" look. The only parameter it accepts determines the variance factor -- how far from the original pixel's value the new value should be allowed to variate.
		iluNoise
		//This function pixellizes an image, making it appear blocky. This function can be used for some neat effects, such as an interesting screen clearing effect.
		iluPixelize
		//iluSharpen "sharpens" the current image, making the edges more noticeable and the whole image "less blurry". Since the original has clearly-defined edges, the sharpen filter has no noticeable effect, so we decided not to include the image to save space.
		iluSharpen*/
	}
	if(ErrorCheck())
	{
		return false;
	}
	return filterApplied;
}
void Texture::CreateFromSurface(SDL_Surface* surface)
{
	mFileName = "CreateFromSurface";
	mWidth = surface->w;
	mHeight = surface->h;
	// Generate a GL texture
	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, surface->pixels);
	// Use linear filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
void Texture::CreateForRendering(int width, int height, unsigned int format)
{
	mFileName = "CreateForRendering";
	mWidth = width;
	mHeight = height;
	// Create the texture id
	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);
	// Set the image width/height with null initial data
	glTexImage2D(GL_TEXTURE_2D, 0, format, mWidth, mHeight, 0, GL_RGB, GL_FLOAT, nullptr);
	// For a texture we'll render to, just use nearest neighbor
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}
void Texture::SetActive(int index)
{
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, mTextureID);
}