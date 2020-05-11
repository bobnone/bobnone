//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "SplineActor.h"
#include "MeshComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "SplineCamera.h"
#include "MoveComponent.h"

SplineActor::SplineActor(Game* game):Actor(game)
{
	//MeshComponent* mc = new MeshComponent(this);
	//mc->setMesh(game->renderer()->getMesh("Assets/RacingCar.gpmesh"));
	//setPosition(Vector3(0.0f, 0.0f, -100.0f));
	cameraComp_ = new SplineCamera(this);
	//Create a spline
	Spline path;
	path.controlPoints_.emplace_back(vector3::Zero);
	for(int i = 0; i < 5; i++)
	{
		if(i % 2 == 0)
		{
			path.controlPoints_.emplace_back(vector3(300.0f * (i + 1), 300.0f, 300.0f));
		}
		else
		{
			path.controlPoints_.emplace_back(vector3(300.0f * (i + 1), 0.0f, 0.0f));
		}
	}
	cameraComp_->setSpline(path);
	cameraComp_->setPaused(false);
}
void SplineActor::actorInput(const uint8_t* keys)
{
	//EMPTY:
}
void SplineActor::restartSpline()
{
	cameraComp_->restart();
}