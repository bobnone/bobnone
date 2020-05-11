//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "BallActor.h"
#include "Game.h"
#include "Renderer.h"
#include "Shader.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "BallMove.h"
#include "AudioComponent.h"
#include "JsonHelper.h"

BallActor::BallActor(Game* game): Actor(game), lifeSpan_(2.0f)
{
	//setScale(10.0f);
	MeshComponent* mc = new MeshComponent(this);
	Renderer* renderer = game->renderer();
	Mesh* mesh = renderer->getMesh("Assets/Sphere.gpmesh");
	mc->setMesh(mesh);
	mc->setShader(renderer->getShader("Mesh"));
	BallMove* move = new BallMove(this);
	move->setForwardSpeed(1500.0f);
	audioComp_ = new AudioComponent(this);
}
void BallActor::updateActor(float deltaTime)
{
	Actor::updateActor(deltaTime);
	lifeSpan_ -= deltaTime;
	if(lifeSpan_ < 0.0f)
	{
		setState(ACTOR_DEAD);
	}
}
void BallActor::hitTarget()
{
	audioComp_->playEvent("event:/Ding");
}
void BallActor::loadProperties(const rapidjson::Value& inObj)
{
	Actor::loadProperties(inObj);
	JsonHelper::getFloat(inObj, "lifespan", lifeSpan_);
}
void BallActor::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const
{
	Actor::saveProperties(alloc, inObj);
	JsonHelper::addFloat(alloc, inObj, "lifespan", lifeSpan_);
}