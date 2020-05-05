// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Component.h"

class MeshComponent : public Component
{
public:
	MeshComponent(class Actor* owner, bool isSkeletal = false);
	~MeshComponent();
	// Draw this mesh component
	virtual void Draw();
	// Draw this mesh component with a custom shader
	virtual void Draw(class Shader* shader);
	// Set the mesh/texture index used by mesh component
	virtual void SetMesh(class Mesh* mesh)
	{
		mMesh = mesh;
	}
	class Mesh* GetMesh()
	{
		return mMesh;
	}
	void SetShader(class Shader* shader);
	class Shader* GetShader()
	{
		return mShader;
	}
	void SetTextureIndex(size_t index)
	{
		mTextureIndex = index;
	}
	size_t GetTextureIndex()
	{
		return mTextureIndex;
	}
	void SetVisible(bool visible)
	{
		mVisible = visible;
	}
	bool GetVisible() const
	{
		return mVisible;
	}
	bool GetIsSkeletal() const
	{
		return mIsSkeletal;
	}
	TypeID GetType() const override
	{
		return TMeshComponent;
	}
	void LoadProperties(const rapidjson::Value& inObj) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const override;
protected:
	class Mesh* mMesh;
	class Shader* mShader;
	size_t mTextureIndex;
	bool mVisible;
	bool mIsSkeletal;
};