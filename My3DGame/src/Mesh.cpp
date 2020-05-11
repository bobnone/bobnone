//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "Mesh.h"
#include "Renderer.h"
#include "LevelLoader.h"
#include <fstream>

namespace
{
	union Vertex
	{
		float f;
		uint8_t b[4];
	};
	const int BinaryVersion = 1;
	struct MeshBinHeader
	{
		//Signature for file type
		char signature_[4] = { 'G', 'M', 'S', 'H' };
		//Version
		uint32_t version_ = BinaryVersion;
		//Vertex layout type
		VertexArray::Layout layout_ = VertexArray::PosNormTex;
		// Info about how many of each we have
		uint32_t numTextures_ = 0;
		uint32_t numVerts_ = 0;
		uint32_t numIndices_ = 0;
		// Box/radius of mesh, used for collision
		AABB box_{vector3::Zero, vector3::Zero};
		//std::string shaderName_ = "Mesh";
		float radius_ = 0.0f;
		float specularPower_ = 100.0f;
	};
}
Mesh::Mesh():box_(vector3::Infinity, vector3::NegInfinity), vertexArray_(nullptr), radius_(0.0f), specularPower_(100.0f)
{
	//EMPTY:
}
Mesh::~Mesh()
{
	//EMPTY:
}
bool Mesh::load(const std::string& fileName, Renderer* renderer)
{
	fileName_ = fileName;
	//Try loading the binary file first
	if(loadBinary(fileName + ".bin", renderer))
	{
		return true;
	}
	rapidjson::Document doc;
	if(!LevelLoader::loadJSON(fileName, doc))
	{
		SDL_Log("Failed to load mesh %s", fileName.c_str());
		return false;
	}
	int ver = doc["version"].GetInt();
	//Check the version
	if(ver != 1)
	{
		SDL_Log("Mesh %s not version 1", fileName.c_str());
		return false;
	}
	//Set the vertex layout/size based on the format in the file
	VertexArray::Layout layout = VertexArray::PosNormTex;
	size_t vertSize = 8;
	std::string vertexFormat = doc["vertexformat"].GetString();
	if(vertexFormat == "PosNormSkinTex")
	{
		layout = VertexArray::PosNormSkinTex;
		//This is the number of "Vertex" unions, which is 8 + 2 (for skinning)s
		vertSize = 10;
	}
	//Load textures
	const rapidjson::Value& textures = doc["textures"];
	if(!textures.IsArray() || textures.Size() < 1)
	{
		SDL_Log("Mesh %s has no textures, there should be at least one", fileName.c_str());
		return false;
	}
	specularPower_ = static_cast<float>(doc["specularPower"].GetDouble());
	std::vector<std::string> textureNames;
	for(rapidjson::SizeType i = 0; i < textures.Size(); i++)
	{
		//Is this texture already loaded?
		std::string texName = textures[i].GetString();
		textureNames.emplace_back(texName);
		Texture* t = renderer->getTexture(texName);
		if(t == nullptr)
		{
			//If it's null, use the default texture
			t = renderer->getTexture("Assets/Default.png");
		}
		textures_.emplace_back(t);
	}
	//Load in the vertices
	const rapidjson::Value& vertsJson = doc["vertices"];
	if(!vertsJson.IsArray() || vertsJson.Size() < 1)
	{
		SDL_Log("Mesh %s has no vertices", fileName.c_str());
		return false;
	}
	std::vector<Vertex> vertices;
	vertices.reserve(vertsJson.Size() * vertSize);
	radius_ = 0.0f;
	for(rapidjson::SizeType i = 0; i < vertsJson.Size(); i++)
	{
		//For now, just assume we have 8 elements
		const rapidjson::Value& vert = vertsJson[i];
		if(!vert.IsArray())
		{
			SDL_Log("Unexpected vertex format for %s", fileName.c_str());
			return false;
		}
		vector3 pos(vert[0].GetDouble(), vert[1].GetDouble(), vert[2].GetDouble());
		radius_ = Math::Max(radius_, pos.Length2());
		box_.updateMinMax(pos);
		if(layout == VertexArray::PosNormTex)
		{
			Vertex v;
			//Add the floats
			for(rapidjson::SizeType j = 0; j < vert.Size(); j++)
			{
				v.f = static_cast<float>(vert[j].GetDouble());
				vertices.emplace_back(v);
			}
		}
		else if(layout == VertexArray::PosNormSkinTex)
		{
			Vertex v;
			//Add position/normal
			for(rapidjson::SizeType j = 0; j < 6; j++)
			{
				v.f = static_cast<float>(vert[j].GetDouble());
				vertices.emplace_back(v);
			}
			//Add skin information
			for(rapidjson::SizeType j = 6; j < 14; j += 4)
			{
				v.b[0] = vert[j].GetUint();
				v.b[1] = vert[j + 1].GetUint();
				v.b[2] = vert[j + 2].GetUint();
				v.b[3] = vert[j + 3].GetUint();
				vertices.emplace_back(v);
			}
			//Add texture coordinates
			for(rapidjson::SizeType j = 14; j < vert.Size(); j++)
			{
				v.f = vert[j].GetDouble();
				vertices.emplace_back(v);
			}
		}
		else
		{
			return false;
		}
	}
	//We were computing length squared earlier
	radius_ = Math::Sqrt(radius_);
	//Load in the indices
	const rapidjson::Value& indJson = doc["indices"];
	if(!indJson.IsArray() || indJson.Size() < 1)
	{
		SDL_Log("Mesh %s has no indices", fileName.c_str());
		return false;
	}
	std::vector<unsigned int> indices;
	indices.reserve(indJson.Size() * 3);
	for(rapidjson::SizeType i = 0; i < indJson.Size(); i++)
	{
		const rapidjson::Value& ind = indJson[i];
		if(!ind.IsArray() || ind.Size() != 3)
		{
			SDL_Log("Invalid indices for %s", fileName.c_str());
			return false;
		}
		indices.emplace_back(ind[0].GetUint());
		indices.emplace_back(ind[1].GetUint());
		indices.emplace_back(ind[2].GetUint());
	}
	//Now create a vertex array
	unsigned int numVerts = static_cast<unsigned>(vertices.size()) / vertSize;
	vertexArray_ = new VertexArray(vertices.data(), numVerts, layout, indices.data(), static_cast<unsigned>(indices.size()));
	//Save the binary mesh
	saveBinary(fileName + ".bin", vertices.data(), numVerts, layout, indices.data(), static_cast<unsigned>(indices.size()), textureNames, box_, radius_, specularPower_);
	return true;
}
void Mesh::unload()
{
	delete vertexArray_;
	vertexArray_ = nullptr;
}
Texture* Mesh::getTexture(size_t index)
{
	if (index < textures_.size())
	{
		return textures_[index];
	}
	else
	{
		return nullptr;
	}
}
void Mesh::saveBinary(const std::string& fileName, const void* verts, uint32_t numVerts, VertexArray::Layout layout, const uint32_t* indices, uint32_t numIndices, const std::vector<std::string>& textureNames, const AABB& box, float radius, float specularPower)
{
	//Create header struct
	MeshBinHeader header;
	header.layout_ = layout;
	header.numTextures_ = static_cast<unsigned>(textureNames.size());
	header.numVerts_ = numVerts;
	header.numIndices_ = numIndices;
	header.box_ = box;
	//header.shaderName_ = shaderName;
	header.radius_ = radius;
	//Open binary file for writing
	std::ofstream outFile(fileName, std::ios::out | std::ios::binary);
	if (outFile.is_open())
	{
		//Write the header
		outFile.write(reinterpret_cast<char*>(&header), sizeof(header));
		//For each texture, we need to write the size of the name
		//followed by the string (null-terminated)
		for(const auto& tex : textureNames)
		{
			//Note: Assumes file names won't have more than 32k characters
			uint16_t nameSize = static_cast<uint16_t>(tex.length()) + 1;
			outFile.write(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
			outFile.write(tex.c_str(), nameSize - 1);
			outFile.write("\0", 1);
		}
		//Figure out number of bytes for each vertex, based on layout
		unsigned vertexSize = VertexArray::getVertexSize(layout);
		//Write vertices
		outFile.write(reinterpret_cast<const char*>(verts), numVerts * vertexSize);
		//Write indices
		outFile.write(reinterpret_cast<const char*>(indices), numIndices * sizeof(uint32_t));
	}
}
bool Mesh::loadBinary(const std::string& fileName, Renderer* renderer)
{
	std::ifstream inFile(fileName, std::ios::in | std::ios::binary);
	if(inFile.is_open())
	{
		//Read in header
		MeshBinHeader header;
		inFile.read(reinterpret_cast<char*>(&header), sizeof(header));
		//Validate the header signature and version
		char* sig = header.signature_;
		if (sig[0] != 'G' || sig[1] != 'M' || sig[2] != 'S' || sig[3] != 'H' || header.version_ != BinaryVersion)
		{
			return false;
		}
		//Read in the texture file names
		for(uint32_t i = 0; i < header.numTextures_; i++)
		{
			//Get the file name size
			uint16_t nameSize = 0;
			inFile.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
			//Make a buffer of this size
			char* texName = new char[nameSize];
			//Read in the texture name
			inFile.read(texName, nameSize);
			//Get this texture
			Texture* t = renderer->getTexture(texName);
			if(t == nullptr)
			{
				//If it's null, use the default texture
				t = renderer->getTexture("Assets/Default.png");
			}
			textures_.emplace_back(t);
			delete[] texName;
		}
		//Now read in the vertices
		unsigned vertexSize = VertexArray::getVertexSize(header.layout_);
		char* verts = new char[header.numVerts_ * vertexSize];
		inFile.read(verts, header.numVerts_ * vertexSize);
		//Now read in the indices
		uint32_t* indices = new uint32_t[header.numIndices_];
		inFile.read(reinterpret_cast<char*>(indices), header.numIndices_ * sizeof(uint32_t));
		//Now create the vertex array
		vertexArray_ = new VertexArray(verts, header.numVerts_, header.layout_, indices, header.numIndices_);
		//Cleanup memory
		delete[] verts;
		delete[] indices;
		//Set mBox/shader/mRadius/specular from header
		box_ = header.box_;
		//shaderName_ = header.shaderName_;
		radius_ = header.radius_;
		specularPower_ = header.specularPower_;
		return true;
	}
	return false;
}