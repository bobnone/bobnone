//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "Ship.h"
#include "SpriteComponent.h"
#include "Game.h"
#include "Laser.h"
#include "InputSystem.h"

Ship::Ship(Game* game):Actor(game), speed_(400.0f), laserCooldown_(0.0f)
{
	//Create a sprite component
	SpriteComponent* sc = new SpriteComponent(this, 150);
	sc->setTexture(game->renderer()->getTexture("Assets/Ship.png"));
}
void Ship::updateActor(float deltaTime)
{
	laserCooldown_ -= deltaTime;
	//Update position based on velocity
	vector2 pos = position();
	pos += velocityDir_ * speed_ * deltaTime;
	setPosition(pos);
	//Update rotation
	setRotation(Math::Atan2(rotationDir_.y, rotationDir_.x));
}
void Ship::actorInput(const InputState& state)
{
	if(state.Controller.rightTrigger() > 0.25f && laserCooldown_ <= 0.0f)
	{
		//Create a laser and set its position/rotation to mine
		Laser* laser = new Laser(game());
		laser->setPosition(position());
		laser->setRotation(rotation());
		//Reset laser cooldown (quarter second)
		laserCooldown_ = 0.25f;
	}
	if(state.Controller.isConnected())
	{
		velocityDir_ = state.Controller.leftStick();
		if(!Math::NearZero(state.Controller.rightStick().Length()))
		{
			rotationDir_ = state.Controller.rightStick();
		}
	}
}