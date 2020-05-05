// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "MeshComponent.h"
#include "Shader.h"
#include "Mesh.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "JsonHelper.h"

MeshComponent::MeshComponent(Actor* owner, bool isSkeletal):Component(owner), mMesh(nullptr), mShader(nullptr), mTextureIndex(0), mVisible(true), mIsSkeletal(isSkeletal)
{
	mOwner->GetGame()->GetRenderer()->AddMeshComp(this);
}
MeshComponent::~MeshComponent()
{
	mOwner->GetGame()->GetRenderer()->RemoveMeshComp(this);
}
void MeshComponent::Draw()
{
	if (mMesh && mShader)
	{
		// Set the world transform
		mShader->SetMatrixUniform("uWorldTransform", mOwner->GetWorldTransform());
		// Set specular power
		mShader->SetFloatUniform("uSpecularPower", mMesh->GetSpecularPower());
		// Set the active texture
		Texture* t = mMesh->GetTexture(mTextureIndex);
		if (t)
		{
			t->SetActive();
		}
		// Set the mesh's vertex array as active
		VertexArray* va = mMesh->GetVertexArray();
		va->SetActive();
		// Draw
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}
void MeshComponent::Draw(Shader* shader)
{
	if (mMesh)
	{
		// Set the world transform
		shader->SetMatrixUniform("uWorldTransform", mOwner->GetWorldTransform());
		// Set specular power
		shader->SetFloatUniform("uSpecularPower", mMesh->GetSpecularPower());
		// Set the active texture
		Texture* t = mMesh->GetTexture(mTextureIndex);
		if (t)
		{
			t->SetActive();
		}
		// Set the mesh's vertex array as active
		VertexArray* va = mMesh->GetVertexArray();
		va->SetActive();
		// Draw
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}
void MeshComponent::SetShader(Shader* shader)
{
	mOwner->GetGame()->GetRenderer()->UnlinkMesh(this);// unlink currtent shader if one is linked
	mShader = shader;
	mOwner->GetGame()->GetRenderer()->LinkMeshToShader(this, mShader);
}
void MeshComponent::LoadProperties(const rapidjson::Value& inObj)
{
	Component::LoadProperties(inObj);
	std::string meshFile;
	if (JsonHelper::GetString(inObj, "meshFile", meshFile))
	{
		SetMesh(mOwner->GetGame()->GetRenderer()->GetMesh(meshFile));
	}
	int index;
	if (JsonHelper::GetInt(inObj, "textureIndex", index))
	{
		mTextureIndex = static_cast<size_t>(index);
	}
	JsonHelper::GetBool(inObj, "visible", mVisible);
	JsonHelper::GetBool(inObj, "isSkeletal", mIsSkeletal);
}
void MeshComponent::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const
{
	Component::SaveProperties(alloc, inObj);
	if (mMesh)
	{
		JsonHelper::AddString(alloc, inObj, "meshFile", mMesh->GetFileName());
	}
	if (mShader)
	{
		JsonHelper::AddString(alloc, inObj, "vertexShaderFile", mShader->GetVertexFile());
		JsonHelper::AddString(alloc, inObj, "fragmentShaderFile", mShader->GetFragmentFile());
	}
	JsonHelper::AddInt(alloc, inObj, "textureIndex", static_cast<int>(mTextureIndex));
	JsonHelper::AddBool(alloc, inObj, "visible", mVisible);
	JsonHelper::AddBool(alloc, inObj, "isSkeletal", mIsSkeletal);
}