//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "CameraActor.h"
#include "MoveComponent.h"
#include "SDL/SDL_scancode.h"
#include "Renderer.h"
#include "AudioSystem.h"
#include "Game.h"
#include "Mesh.h"
#include "AudioComponent.h"
#include "MeshComponent.h"

CameraActor::CameraActor(Game* game):Actor(game)
{
	moveComp_ = new MoveComponent(this);
	audioComp_ = new AudioComponent(this);
	MeshComponent* mc = new MeshComponent(this);
	mc->setMesh(game->renderer()->getMesh("Assets/Sphere.gpmesh"));
	lastFootstep_ = 0.0f;
	footstep_ = audioComp_->playEvent("event:/Footstep");
	footstep_.setPaused(true);
}
void CameraActor::updateActor(float deltaTime)
{
	Actor::updateActor(deltaTime);
	//Play the footstep if we're moving and haven't recently
	lastFootstep_ -= deltaTime;
	if(!Math::NearZero(moveComp_->forwardSpeed()) && lastFootstep_ <= 0.0f)
	{
		footstep_.setPaused(false);
		footstep_.restart();
		lastFootstep_ = 0.5f;
	}
	// Compute new camera from this actor
	cameraPos_ = position() - getForward() * 200.0f + vector3::UnitZ * 100.0f;
	vector3 target = position() + getForward() * 100.0f;
	vector3 up = vector3::UnitZ;
	matrix4 view = matrix4::CreateLookAt(cameraPos_, target, up);
	game()->renderer()->setViewMatrix(view);
	game()->audioSystem()->setListener(this, view, position_);
}
void CameraActor::actorInput(const uint8_t* keys)
{
	float forwardSpeed = 0.0f;
	float angularSpeed = 0.0f;
	//wasd movement
	if (keys[SDL_SCANCODE_W])
	{
		forwardSpeed += 300.0f;
	}
	if (keys[SDL_SCANCODE_S])
	{
		forwardSpeed -= 300.0f;
	}
	if (keys[SDL_SCANCODE_A])
	{
		angularSpeed -= Math::TwoPi;
	}
	if (keys[SDL_SCANCODE_D])
	{
		angularSpeed += Math::TwoPi;
	}
	moveComp_->setForwardSpeed(forwardSpeed);
	moveComp_->setAngularXSpeed(angularSpeed);
}
void CameraActor::setFootstepSurface(float value)
{
	//Pause here because the way I setup the parameter in FMOD
	//changing it will play a footstep
	footstep_.setPaused(true);
	footstep_.setParameter("Surface", value);
}