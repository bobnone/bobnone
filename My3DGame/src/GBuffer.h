//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include <vector>

class GBuffer
{
public:
	//Different types of data stored in the G-buffer
	enum Type
	{
		GBUFFER_DIFFUSE = 0,
		GBUFFER_SPECULAR,
		GBUFFER_NORMAL,
		GBUFFER_WORLDPOSITION,
		NUM_GBUFFER_TEXTURES
	};
	GBuffer();
	~GBuffer();
	//Create/destroy the G-buffer
	bool create(int width, int height);
	void destroy();
	//Get the texture for a specific type of data
	class Texture* getTexture(Type type);
	//Get the frame-buffer object ID
	unsigned int bufferID() const
	{
		return bufferID_;
	}
	//Setup all the G-buffer textures for sampling
	void setTexturesActive();
private:
	//Textures associated with G-buffer
	std::vector<class Texture*> textures_;
	//Frame buffer object ID
	unsigned int bufferID_;
};