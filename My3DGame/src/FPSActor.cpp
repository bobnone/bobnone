//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "FPSActor.h"
#include "MoveComponent.h"
#include "SDL/SDL_scancode.h"
#include "Renderer.h"
#include "AudioSystem.h"
#include "Game.h"
#include "AudioComponent.h"
#include "FPSCamera.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "Math.h"
#include "BallActor.h"
#include "BoxComponent.h"
#include "PlaneActor.h"

FPSActor::FPSActor(Game* game):Actor(game)
{
	moveComp_ = new MoveComponent(this);
	audioComp_ = new AudioComponent(this);
	lastFootstep_ = 0.0f;
	footstep_ = audioComp_->playEvent("event:/Footstep");
	footstep_.setPaused(true);
	cameraComp_ = new FPSCamera(this);
	fPSModel_ = new Actor(game);
	fPSModel_->setScale(0.75f);
	meshComp_ = new MeshComponent(fPSModel_);
	meshComp_->setMesh(game->renderer()->getMesh("Assets/Rifle.gpmesh"));
	//Add a box component
	boxComp_ = new BoxComponent(this);
	AABB myBox(Vector3(-25.0f, -25.0f, -87.5f), Vector3(25.0f, 25.0f, 87.5f));
	boxComp_->setObjectBox(myBox);
	boxComp_->setShouldRotate(false);
}
void FPSActor::updateActor(float deltaTime)
{
	Actor::updateActor(deltaTime);
	fixCollisions();
	//Play the footstep if we're moving and haven't recently
	lastFootstep_ -= deltaTime;
	if((!Math::nearZero(moveComp_->forwardSpeed()) || !Math::nearZero(moveComp_->strafeSpeed())) && lastFootstep_ <= 0.0f)
	{
		footstep_.setPaused(false);
		footstep_.restart();
		lastFootstep_ = 0.5f;
	}
	//Update position of FPS model relative to actor position
	const Vector3 modelOffset(Vector3(10.0f, 10.0f, -10.0f));
	Vector3 modelPos = position();
	modelPos += getForward() * modelOffset.x;
	modelPos += getRight() * modelOffset.y;
	modelPos.z += modelOffset.z;
	fPSModel_->setPosition(modelPos);
	//Initialize rotation to actor rotation
	Quaternion q = rotation();
	//Rotate by pitch from camera
	q = Math::concatenate(q, Quaternion(getRight(), cameraComp_->pitch()));
	fPSModel_->setRotation(q);
}
void FPSActor::actorInput(const uint8_t* keys)
{
	float forwardSpeed = 0.0f;
	float strafeSpeed = 0.0f;
	//wasd movement
	if(keys[SDL_SCANCODE_W])
	{
		forwardSpeed += 400.0f;
	}
	if(keys[SDL_SCANCODE_S])
	{
		forwardSpeed -= 400.0f;
	}
	if(keys[SDL_SCANCODE_A])
	{
		strafeSpeed -= 400.0f;
	}
	if(keys[SDL_SCANCODE_D])
	{
		strafeSpeed += 400.0f;
	}
	moveComp_->setForwardSpeed(forwardSpeed);
	moveComp_->setStrafeSpeed(strafeSpeed);
	//Mouse movement
	//Get relative movement from SDL
	int x, y;
	SDL_GetRelativeMouseState(&x, &y);
	//Assume mouse movement is usually between -500 and +500
	const int maxMouseSpeed = 500;
	//Rotation/sec at maximum speed
	const float maxAngularSpeed = Math::PI * 8;
	float angularSpeed = 0.0f;
	if(x != 0)
	{
		//Convert to ~[-1.0, 1.0]
		angularSpeed = static_cast<float>(x) / maxMouseSpeed;
		//Multiply by rotation/sec
		angularSpeed *= maxAngularSpeed;
	}
	moveComp_->setAngularXSpeed(angularSpeed);
	//Compute pitch
	const float maxPitchSpeed = Math::PI * 8;
	float pitchSpeed = 0.0f;
	if(y != 0)
	{
		//Convert to ~[-1.0, 1.0]
		pitchSpeed = static_cast<float>(y) / maxMouseSpeed;
		pitchSpeed *= maxPitchSpeed;
	}
	cameraComp_->setPitchSpeed(pitchSpeed);
}
void FPSActor::shoot()
{
	//Get direction vector
	Vector3 start, dir;
	game()->renderer()->getScreenDirection(start, dir);
	//Spawn a ball
	BallActor* ball = new BallActor(game());
	ball->setParent(this);
	ball->setPosition(start + dir*20.0f);
	//Rotate the ball to face new direction
	ball->rotateToNewForward(dir);
	//Play shooting sound
	audioComp_->playEvent("event:/Shot");
}
void FPSActor::setFootstepSurface(float value)
{
	//Pause here because the way I setup the parameter in FMOD
	//changing it will play a footstep
	footstep_.setPaused(true);
	footstep_.setParameter("Surface", value);
}
void FPSActor::setVisible(bool visible)
{
	meshComp_->setVisible(visible);
}
void FPSActor::fixCollisions()
{
	//Need to recompute my world transform to update world box
	computeWorldTransform();
	const AABB& playerBox = boxComp_->worldBox();
	Vector3 pos = position();
	auto& planes = game()->getPlanes();
	for(auto pa : planes)
	{
		//Do we collide with this PlaneActor?
		const AABB& planeBox = pa->getBox()->getWorldBox();
		if(intersect(playerBox, planeBox))
		{
			//Calculate all our differences
			float dx1 = planeBox.max_.x - playerBox.min_.x;
			float dx2 = planeBox.min_.x - playerBox.max_.x;
			float dy1 = planeBox.max_.y - playerBox.min_.y;
			float dy2 = planeBox.min_.y - playerBox.max_.y;
			float dz1 = planeBox.max_.z - playerBox.min_.z;
			float dz2 = planeBox.min_.z - playerBox.max_.z;
			//Set dx to whichever of dx1/dx2 have a lower abs
			float dx = Math::abs(dx1) < Math::abs(dx2) ? dx1 : dx2;
			//Ditto for dy
			float dy = Math::abs(dy1) < Math::abs(dy2) ? dy1 : dy2;
			//Ditto for dz
			float dz = Math::abs(dz1) < Math::abs(dz2) ? dz1 : dz2;
			//Whichever is closest, adjust x/y position
			if(Math::abs(dx) <= Math::abs(dy) && Math::abs(dx) <= Math::abs(dz))
			{
				pos.x += dx;
			}
			else if(Math::abs(dy) <= Math::abs(dx) && Math::abs(dy) <= Math::abs(dz))
			{
				pos.y += dy;
			}
			else
			{
				pos.z += dz;
			}
			//Need to set position and update box component
			setPosition(pos);
			boxComp_->onUpdateWorldTransform();
		}
	}
}