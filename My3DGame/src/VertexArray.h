//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once

class VertexArray
{
public:
	//Different supported vertex layouts
	enum Layout
	{
		PosNormTex,
		PosNormSkinTex,
		PosNormTexRGB
	};

	VertexArray(const void* verts, unsigned int numVerts, Layout layout, const unsigned int* indices, unsigned int numIndices);
	~VertexArray();
	void setActive();
	unsigned int numIndices() const
	{
		return numIndices_;
	}
	unsigned int numVerts() const
	{
		return numVerts_;
	}
	static unsigned int getVertexSize(VertexArray::Layout layout);
private:
	//How many vertices in the vertex buffer?
	unsigned int numVerts_;
	//How many indices in the index buffer
	unsigned int numIndices_;
	//OpenGL ID of the vertex buffer
	unsigned int vertexBuffer_;
	//OpenGL ID of the index buffer
	unsigned int indexBuffer_;
	//OpenGL ID of the vertex array object
	unsigned int vertexArray_;
};