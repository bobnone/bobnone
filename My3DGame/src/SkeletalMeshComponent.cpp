//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "SkeletalMeshComponent.h"
#include "Mesh.h"
#include "Game.h"
#include "Texture.h"
#include "JsonHelper.h"

SkeletalMeshComponent::SkeletalMeshComponent(Actor* owner):MeshComponent(owner, true), skeleton_(nullptr), animation_(nullptr), animPlayRate_(0.0f), animTime_(0.0f)
{
	//EMPTY:
}
void SkeletalMeshComponent::draw(Shader* shader)
{
	if(mesh_)
	{
		//Set the world transform
		shader->setMatrixUniform("uWorldTransform", owner_->worldTransform());
		//Set the matrix palette
		shader->setMatrixUniforms("uMatrixPalette", &palette_.entry_[0], MAX_SKELETON_BONES);
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
void SkeletalMeshComponent::update(float deltaTime)
{
	if(animation_ && skeleton_)
	{
		animTime_ += deltaTime * animPlayRate_;
		//Wrap around mAnimTime if past duration
		while(animTime_ > animation_->duration())
		{
			animTime_ -= animation_->duration();
		}
		//Recompute matrix palette
		computeMatrixPalette();
	}
}
float SkeletalMeshComponent::playAnimation(Animation* anim, float playRate)
{
	animation_ = anim;
	animTime_ = 0.0f;
	animPlayRate_ = playRate;
	if(!animation_)
	{
		return 0.0f;
	}
	computeMatrixPalette();
	return animation_->duration();
}
void SkeletalMeshComponent::loadProperties(const rapidjson::Value& inObj)
{
	MeshComponent::loadProperties(inObj);
	std::string skelFile;
	if(JsonHelper::getString(inObj, "skelFile", skelFile))
	{
		setSkeleton(owner_->game()->getSkeleton(skelFile));
	}
	std::string animFile;
	if(JsonHelper::getString(inObj, "animFile", animFile))
	{
		playAnimation(owner_->game()->getAnimation(animFile));
	}
	JsonHelper::getFloat(inObj, "animPlayRate", animPlayRate_);
	JsonHelper::getFloat(inObj, "animTime", animTime_);
}
void SkeletalMeshComponent::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const
{
	MeshComponent::saveProperties(alloc, inObj);
	if(skeleton_)
	{
		JsonHelper::addString(alloc, inObj, "skelFile", skeleton_->fileName());
	}
	if(animation_)
	{
		JsonHelper::addString(alloc, inObj, "animFile", animation_->fileName());
	}
	JsonHelper::addFloat(alloc, inObj, "animPlayRate", animPlayRate_);
	JsonHelper::addFloat(alloc, inObj, "animTime", animTime_);
}
void SkeletalMeshComponent::computeMatrixPalette()
{
	const std::vector<Matrix4x4>& globalInvBindPoses = skeleton_->globalInvBindPoses();
	std::vector<Matrix4x4> currentPoses;
	animation_->getGlobalPoseAtTime(currentPoses, skeleton_, animTime_);
	//Setup the palette for each bone
	for(size_t i = 0; i < skeleton_->getNumBones(); i++)
	{
		//Global inverse bind pose matrix times current pose matrix
		palette_.entry_[i] = globalInvBindPoses[i] * currentPoses[i];
	}
}