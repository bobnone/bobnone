//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "PointLightComponent.h"
#include "Shader.h"
#include "Game.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Actor.h"
#include "JsonHelper.h"

PointLightComponent::PointLightComponent(Actor* owner) :Component(owner), innerRadius_(100.0f), outerRadius_(200.0f)
{
	owner->game()->renderer()->addPointLight(this);
}
PointLightComponent::~PointLightComponent()
{
	owner_->game()->renderer()->removePointLight(this);
}
void PointLightComponent::draw(Shader* shader, Mesh* mesh)
{
	//We assume, coming into this function, that the shader is active
	//and the sphere mesh is active
	//World transform is scaled to the outer radius (divided by the mesh radius)
	//and positioned to the world position
	Matrix4x4 scale = Math::createScaleMatrix(owner_->scale() * outerRadius_ / mesh->radius());
	Matrix4x4 trans = Math::createTranslationMatrix(owner_->position());
	Matrix4x4 worldTransform = scale * trans;
	shader->setMatrixUniform("uWorldTransform", worldTransform);
	//Set point light shader constants
	shader->setVectorUniform("uPointLight.worldPos_", owner_->position());
	shader->setVectorUniform("uPointLight.diffuseColor_", diffuseColor_);
	shader->setVectorUniform("uPointLight.specularColor_", specularColor_);
	shader->setVectorUniform("uPointLight.specularPower_", specularPower_);
	shader->setFloatUniform("uPointLight.innerRadius_", innerRadius_);
	shader->setFloatUniform("uPointLight.outerRadius_", outerRadius_);
	//Draw the sphere
	glDrawElements(GL_TRIANGLES, mesh->vertexArray()->numIndices(), GL_UNSIGNED_INT, nullptr);
}
void PointLightComponent::loadProperties(const rapidjson::Value& inObj)
{
	Component::loadProperties(inObj);
	JsonHelper::getVector3(inObj, "color", diffuseColor_);
	JsonHelper::getVector3(inObj, "diffuseColor", diffuseColor_);
	JsonHelper::getVector3(inObj, "specularColor", specularColor_);
	JsonHelper::getVector3(inObj, "specularPower", specularPower_);
	JsonHelper::getFloat(inObj, "innerRadius", innerRadius_);
	JsonHelper::getFloat(inObj, "outerRadius", outerRadius_);
}
void PointLightComponent::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const
{
	JsonHelper::addVector3(alloc, inObj, "diffuseColor", diffuseColor_);
	JsonHelper::addVector3(alloc, inObj, "specularColor", specularColor_);
	JsonHelper::addVector3(alloc, inObj, "specularPower", specularPower_);
	JsonHelper::addFloat(alloc, inObj, "innerRadius", innerRadius_);
	JsonHelper::addFloat(alloc, inObj, "outerRadius", outerRadius_);
}