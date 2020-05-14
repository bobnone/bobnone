//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include "MeshComponent.h"
#include "MatrixPalette.h"
#include "Shader.h"
#include "Actor.h"
#include "Animation.h"
#include "Skeleton.h"

class SkeletalMeshComponent : public MeshComponent
{
public:
	SkeletalMeshComponent(class Actor* owner);
	//Draw this mesh component
	void draw(class Shader* shader) override;
	void update(float deltaTime) override;
	//Setters
	void setSkeleton(class Skeleton* sk)
	{
		skeleton_ = sk;
	}
	//Play an animation. Returns the length of the animation
	float playAnimation(class Animation* anim, float playRate = 1.0f);
	TypeID getType() const override
	{
		return TSkeletalMeshComponent;
	}
	void loadProperties(const rapidjson::Value& inObj) override;
	void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const override;
protected:
	void computeMatrixPalette();
	struct MatrixPalette palette_;
	class Skeleton* skeleton_;
	class Animation* animation_;
	float animPlayRate_;
	float animTime_;
};