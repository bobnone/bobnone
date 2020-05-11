//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "OrbitActor.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "Game.h"
#include "Renderer.h"
#include "OrbitCamera.h"
#include "MoveComponent.h"

OrbitActor::OrbitActor(Game* game):Actor(game)
{
	meshComp_ = new MeshComponent(this);
	meshComp_->setMesh(game->renderer()->getMesh("Assets/RacingCar.gpmesh"));
	setPosition(vector3(0.0f, 0.0f, -100.0f));
	cameraComp_ = new OrbitCamera(this);
}
void OrbitActor::actorInput(const uint8_t* keys)
{
	//Mouse rotation
	//Get relative movement from SDL
	int x, y;
	Uint32 buttons = SDL_GetRelativeMouseState(&x, &y);
	//Only apply rotation if right-click is held
	if(buttons & SDL_BUTTON(SDL_BUTTON_RIGHT))
	{
		//Assume mouse movement is usually between -500 and +500
		const int maxMouseSpeed = 500;
		//Rotation/sec at maximum speed
		const float maxOrbitSpeed = Math::Pi * 8;
		float yawSpeed = 0.0f;
		if(x != 0)
		{
			//Convert to ~[-1.0, 1.0]
			yawSpeed = static_cast<float>(x) / maxMouseSpeed;
			//Multiply by rotation/sec
			yawSpeed *= maxOrbitSpeed;
		}
		cameraComp_->setYawSpeed(-yawSpeed);
		//Compute pitch
		float pitchSpeed = 0.0f;
		if(y != 0)
		{
			//Convert to ~[-1.0, 1.0]
			pitchSpeed = static_cast<float>(y) / maxMouseSpeed;
			pitchSpeed *= maxOrbitSpeed;
		}
		cameraComp_->setPitchSpeed(pitchSpeed);
	}
}
void OrbitActor::setVisible(bool visible)
{
	meshComp_->setVisible(visible);
}