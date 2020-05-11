//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include <string>
#include "Collision.h"
#include "VertexArray.h"

class Mesh
{
public:
	Mesh();
	~Mesh();
	//Load/unload mesh
	bool load(const std::string& fileName, class Renderer* renderer);
	void unload();
	//Get the vertex array associated with this mesh
	VertexArray* vertexArray()
	{
		return vertexArray_;
	}
	//Get a texture from specified index
	class Texture* getTexture(size_t index);
	//Get file name
	const std::string& fileName() const
	{
		return fileName_;
	}
	//Get object space bounding sphere radius
	float radius() const
	{
		return radius_;
	}
	//Get object space bounding box
	const AABB& box() const
	{
		return box_;
	}
	//Get specular power of mesh
	float specularPower() const
	{
		return specularPower_;
	}
	//Save the mesh in binary format
	void saveBinary(const std::string& fileName, const void* verts, uint32_t numVerts, VertexArray::Layout layout, const uint32_t* indices, uint32_t numIndices, const std::vector<std::string>& textureNames, const AABB& box, float radius, float specularPower);
	//Load in the mesh from binary format
	bool loadBinary(const std::string& fileName, class Renderer* renderer);
private:
	//AABB collision
	AABB box_;
	//Textures associated with this mesh
	std::vector<class Texture*> textures_;
	//Vertex array associated with this mesh
	VertexArray* vertexArray_;
	//Name of mesh file
	std::string fileName_;
	//Stores object space bounding sphere radius
	float radius_;
	//Specular power of surface
	float specularPower_;
};