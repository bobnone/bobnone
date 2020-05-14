//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include "Math/Math.h"
#include "Component.h"

class PointLightComponent: public Component
{
public:
	PointLightComponent(class Actor* owner);
	~PointLightComponent();
	//Draw this point light as geometry
	void draw(class Shader* shader, class Mesh* mesh);
	//Diffuse color
	Vector3 diffuseColor_;
	//Specular color
	Vector3 specularColor_;
	//Specular power
	Vector3 specularPower_;
	//Radius of light
	float innerRadius_;
	float outerRadius_;
	TypeID getType() const override
	{
		return TPointLightComponent;
	}
	void loadProperties(const rapidjson::Value& inObj) override;
	void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const override;
};