//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "GBuffer.h"
#include <GL/glew.h>
#include "Texture.h"

GBuffer::GBuffer(): bufferID_(0)
{
	//EMPTY:
}
GBuffer::~GBuffer()
{
	//EMPTY:
}
bool GBuffer::create(int width, int height)
{
	//Create the frame-buffer object
	glGenFramebuffers(1, &bufferID_);
	glBindFramebuffer(GL_FRAMEBUFFER, bufferID_);
	//Add a depth buffer to this target
	GLuint depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
	//Create textures for each output in the G-buffer
	for(int i = 0; i < NUM_GBUFFER_TEXTURES; i++)
	{
		Texture* tex = new Texture();
		// We want three 32-bit float components for each texture
		tex->createForRendering(width, height, GL_RGB32F);
		textures_.emplace_back(tex);
		//Attach this texture to a color output
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, tex->textureID(), 0);
	}
	//Create a vector of the color attachments
	std::vector<GLenum> attachments;
	for(int i = 0; i < NUM_GBUFFER_TEXTURES; i++)
	{
		attachments.emplace_back(GL_COLOR_ATTACHMENT0 + i);
	}
	//Set the list of buffers to draw to
	glDrawBuffers(static_cast<GLsizei>(attachments.size()), attachments.data());
	//Make sure everything worked
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		destroy();
		return false;
	}
	return true;
}
void GBuffer::destroy()
{
	glDeleteFramebuffers(1, &bufferID_);
	for(Texture* t : textures_)
	{
		t->unload();
		delete t;
	}
}
Texture* GBuffer::getTexture(Type type)
{
	if(textures_.size() > 0)
	{
		return textures_[type];
	}
	else
	{
		return nullptr;
	}
}
void GBuffer::setTexturesActive()
{
	for(int i = 0; i < NUM_GBUFFER_TEXTURES; i++)
	{
		textures_[i]->setActive(i);
	}
}