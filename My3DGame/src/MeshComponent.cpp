//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "MeshComponent.h"
#include "Shader.h"
#include "Mesh.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "JsonHelper.h"

MeshComponent::MeshComponent(Actor* owner, bool isSkeletal):Component(owner), mesh_(nullptr), shader_(nullptr), textureIndex_(0), visible_(true), isSkeletal_(isSkeletal)
{
	owner_->game()->renderer()->addMeshComp(this);
}
MeshComponent::~MeshComponent()
{
	owner_->game()->renderer()->removeMeshComp(this);
}
void MeshComponent::draw()
{
	if(mesh_ && shader_)
	{
		//Set the world transform
		shader_->setMatrixUniform("uWorldTransform", owner_->worldTransform());
		//Set specular power
		shader_->setFloatUniform("uSpecularPower", mesh_->specularPower());
		//Set the active texture
		Texture* t = mesh_->getTexture(textureIndex_);
		if(t)
		{
			t->setActive();
		}
		//Set the mesh's vertex array as active
		VertexArray* va = mesh_->vertexArray();
		va->setActive();
		//Draw
		glDrawElements(GL_TRIANGLES, va->numIndices(), GL_UNSIGNED_INT, nullptr);
	}
}
void MeshComponent::draw(Shader* shader)
{
	if(mesh_)
	{
		//Set the world transform
		shader->setMatrixUniform("uWorldTransform", owner_->worldTransform());
		//Set specular power
		shader->setFloatUniform("uSpecularPower", mesh_->specularPower());
		//Set the active texture
		Texture* t = mesh_->getTexture(textureIndex_);
		if(t)
		{
			t->setActive();
		}
		//Set the mesh's vertex array as active
		VertexArray* va = mesh_->vertexArray();
		va->setActive();
		//Draw
		glDrawElements(GL_TRIANGLES, va->numIndices(), GL_UNSIGNED_INT, nullptr);
	}
}
void MeshComponent::setShader(Shader* shader)
{
	//unlink current shader if one is linked
	owner_->game()->renderer()->unlinkMesh(this);
	shader_ = shader;
	owner_->game()->renderer()->linkMeshToShader(this, shader_);
}
void MeshComponent::loadProperties(const rapidjson::Value& inObj)
{
	Component::loadProperties(inObj);
	std::string meshFile;
	if (JsonHelper::getString(inObj, "meshFile", meshFile))
	{
		setMesh(owner_->game()->renderer()->getMesh(meshFile));
	}
	int index;
	if (JsonHelper::getInt(inObj, "textureIndex", index))
	{
		textureIndex_ = static_cast<size_t>(index);
	}
	JsonHelper::getBool(inObj, "visible", visible_);
	JsonHelper::getBool(inObj, "isSkeletal", isSkeletal_);
}
void MeshComponent::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const
{
	Component::saveProperties(alloc, inObj);
	if(mesh_)
	{
		JsonHelper::addString(alloc, inObj, "meshFile", mesh_->fileName());
	}
	if(shader_)
	{
		JsonHelper::addString(alloc, inObj, "vertexShaderFile", shader_->vertexFile());
		JsonHelper::addString(alloc, inObj, "fragmentShaderFile", shader_->fragmentFile());
	}
	JsonHelper::addInt(alloc, inObj, "textureIndex", static_cast<int>(textureIndex_));
	JsonHelper::addBool(alloc, inObj, "visible", visible_);
	JsonHelper::addBool(alloc, inObj, "isSkeletal", isSkeletal_);
}