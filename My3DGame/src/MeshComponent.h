//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include "Component.h"

class MeshComponent: public Component
{
public:
	MeshComponent(class Actor* owner, bool isSkeletal = false);
	~MeshComponent();
	//Draw this mesh component
	virtual void draw();
	//Draw this mesh component with a custom shader
	virtual void draw(class Shader* shader);
	//Set the mesh/texture index used by mesh component
	virtual void setMesh(class Mesh* mesh)
	{
		mesh_ = mesh;
	}
	class Mesh* mesh()
	{
		return mesh_;
	}
	void setShader(class Shader* shader);
	class Shader* shader()
	{
		return shader_;
	}
	void setTextureIndex(size_t index)
	{
		textureIndex_ = index;
	}
	size_t textureIndex()
	{
		return textureIndex_;
	}
	void setVisible(bool visible)
	{
		visible_ = visible;
	}
	bool visible() const
	{
		return visible_;
	}
	bool isSkeletal() const
	{
		return isSkeletal_;
	}
	TypeID getType() const override
	{
		return TMeshComponent;
	}
	void loadProperties(const rapidjson::Value& inObj) override;
	void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const override;
protected:
	class Mesh* mesh_;
	class Shader* shader_;
	size_t textureIndex_;
	bool visible_;
	bool isSkeletal_;
};