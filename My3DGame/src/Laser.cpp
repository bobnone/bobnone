//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "Laser.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "CircleComponent.h"
#include "Asteroid.h"

Laser::Laser(Game* game):Actor(game), deathTimer_(1.0f)
{
	//Create a sprite component
	SpriteComponent* sc = new SpriteComponent(this);
	sc->setTexture(game->renderer()->getTexture("Assets/Laser.png"));
	// Create a move component, and set a forward speed
	MoveComponent* mc = new MoveComponent(this);
	mc->setForwardSpeed(800.0f);
	// Create a circle component (for collision)
	circle_ = new CircleComponent(this);
	circle_->setRadius(11.0f);
}
void Laser::updateActor(float deltaTime)
{
	//If we run out of time, laser is dead
	deathTimer_ -= deltaTime;
	if(deathTimer_ <= 0.0f)
	{
		setState(ACTOR_DEAD);
	}
	else
	{
		//Do we intersect with an asteroid?
		for(auto ast : game()->getAsteroids())
		{
			if(intersect(*circle_, *(ast->getCircle())))
			{
				//The first asteroid we intersect with,
				//set ourselves and the asteroid to dead
				setState(ACTOR_DEAD);
				ast->setState(ACTOR_DEAD);
				break;
			}
		}
	}
}