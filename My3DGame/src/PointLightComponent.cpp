// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "PointLightComponent.h"
#include "Shader.h"
#include "Game.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Actor.h"
#include "JsonHelper.h"

PointLightComponent::PointLightComponent(Actor* owner) :Component(owner), mInnerRadius(100.0f), mOuterRadius(200.0f)
{
	owner->GetGame()->GetRenderer()->AddPointLight(this);
}
PointLightComponent::~PointLightComponent()
{
	mOwner->GetGame()->GetRenderer()->RemovePointLight(this);
}
void PointLightComponent::Draw(Shader* shader, Mesh* mesh)
{
	// We assume, coming into this function, that the shader is active
	// and the sphere mesh is active
	// World transform is scaled to the outer radius (divided by the mesh radius)
	// and positioned to the world position
	matrix4 scale = matrix4::CreateScale(mOwner->GetScale() * mOuterRadius / mesh->GetRadius());
	matrix4 trans = matrix4::CreateTranslation(mOwner->GetPosition());
	matrix4 worldTransform = scale * trans;
	shader->SetMatrixUniform("uWorldTransform", worldTransform);
	// Set point light shader constants
	shader->SetVectorUniform("uPointLight.mWorldPos", mOwner->GetPosition());
	shader->SetVectorUniform("uPointLight.mDiffuseColor", mDiffuseColor);
	shader->SetVectorUniform("uPointLight.mSpecularColor", mSpecularColor);
	shader->SetVectorUniform("uPointLight.mSpecularPower", mSpecularPower);
	shader->SetFloatUniform("uPointLight.mInnerRadius", mInnerRadius);
	shader->SetFloatUniform("uPointLight.mOuterRadius", mOuterRadius);
	// Draw the sphere
	glDrawElements(GL_TRIANGLES, mesh->GetVertexArray()->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
}
void PointLightComponent::LoadProperties(const rapidjson::Value& inObj)
{
	Component::LoadProperties(inObj);
	JsonHelper::GetVector3(inObj, "color", mDiffuseColor);
	JsonHelper::GetVector3(inObj, "diffuseColor", mDiffuseColor);
	JsonHelper::GetVector3(inObj, "specularColor", mSpecularColor);
	JsonHelper::GetVector3(inObj, "specularPower", mSpecularPower);
	JsonHelper::GetFloat(inObj, "innerRadius", mInnerRadius);
	JsonHelper::GetFloat(inObj, "outerRadius", mOuterRadius);
}
void PointLightComponent::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const
{
	JsonHelper::AddVector3(alloc, inObj, "diffuseColor", mDiffuseColor);
	JsonHelper::AddVector3(alloc, inObj, "specularColor", mSpecularColor);
	JsonHelper::AddVector3(alloc, inObj, "specularPower", mSpecularPower);
	JsonHelper::AddFloat(alloc, inObj, "innerRadius", mInnerRadius);
	JsonHelper::AddFloat(alloc, inObj, "outerRadius", mOuterRadius);
}