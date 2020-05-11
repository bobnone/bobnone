//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE.txt for full details.
//----------------------------------------------------------------

#include "Bullet.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "CircleComponent.h"
#include "Game.h"
#include "Enemy.h"

Bullet::Bullet(class Game* game):Actor(game)
{
	SpriteComponent* sc = new SpriteComponent(this);
	sc->setTexture(game->renderer()->getTexture("Assets/Projectile.png"));
	MoveComponent* mc = new MoveComponent(this);
	mc->setForwardSpeed(400.0f);
	circle_ = new CircleComponent(this);
	circle_->setRadius(5.0f);
	liveTime_ = 1.0f;
}
void Bullet::updateActor(float deltaTime)
{
	Actor::updateActor(deltaTime);
	//Check for collision vs enemies
	for(Enemy* e : game()->getEnemies())
	{
		if(intersect(*circle_, *(e->circle())))
		{
			//We both die on collision
			e->setState(ACTOR_DEAD);
			setState(ACTOR_DEAD);
			break;
		}
	}
	liveTime_ -= deltaTime;
	if(liveTime_ <= 0.0f)
	{
		//Time limit hit, die
		setState(ACTOR_DEAD);
	}
}