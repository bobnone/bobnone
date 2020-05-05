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
	Texture();
	~Texture();
	bool Texture::Load(const std::string& fileName);
	void Unload();
	// Returns true if an error is found with DevIL
	bool ErrorCheck();
	std::wstring Texture::StringToWstring(const std::string& str);
	std::string Texture::WstringToString(const std::wstring& wstr);
	// Returns whether the file is loaded or not
	bool LoadData(const wchar_t* fileName);
	/* Returns whether the file is loaded or not
	Ignores file extensions and uses "type" instead*/
	bool LoadData(const wchar_t* fileName, ILenum type);
	/* Returns whether the file is loaded or not
	"file" points to an open file to read from*/
	bool LoadData(ILHANDLE* file, ILenum type);
	/* Returns whether the file is loaded or not
	"lump points" to block of memory of size "size" to read from*/
	bool LoadData(const void* lump, ILenum type, ILuint size);
	// Returns whether the file is saved or not
	bool SaveData(const wchar_t* fileName);
	/* Returns whether the file is saved or not
	Ignores file extensions and uses "type" instead*/
	bool SaveData(const wchar_t* fileName, ILenum type);
	/* Returns whether the file is saved or not
	"file" points to an open file to write to*/
	bool SaveData(ILHANDLE* file, ILenum type);
	/* Returns whether the file is saved or not
	"lump points" to block of memory of size "size" to write to*/
	bool SaveData(void* lump, ILenum type, ILuint size);
	enum TextureFilter
	{
		FILTER_ALIENIFY = 0,
		FILTER_BIT_1,
		FILTER_BIT_2,
		FILTER_BIT_3,
		FILTER_BLUR_AVG,
		FILTER_BLUR_GAUSSIAN,
		FILTER_EDGE_DETECT_PREWITT,
		FILTER_EDGE_DETECT_SOBEL,
		FILTER_EMBOSS,
		FILTER_GAMMA_CORRECT_CURVE,
		FILTER_NEGATIVE,
		FILTER_NOISE,
		FILTER_PIXELIZE,
		FILTER_SHARPEN
	};
	// Returns a boolean value telling whether the filter was successfully applied
	ILboolean ApplyFilter(TextureFilter filter);
	void CreateFromSurface(struct SDL_Surface* surface);
	void CreateForRendering(int width, int height, unsigned int format);
	void SetActive(int index = 0);
	const std::string& GetFileName() const
	{
		return mFileName;
	}
	GLuint GetTextureID() const
	{
		return mTextureID;
	}
	ILuint GetImageID() const
	{
		return mImageID;
	}
	const int GetWidth() const
	{
		return mWidth;
	}
	const int GetHeight() const
	{
		return mHeight;
	}
private:
	std::string mFileName;
	GLuint mTextureID;
	ILuint mImageID;
	ILinfo* mInfo;
	int mWidth;
	int mHeight;
};