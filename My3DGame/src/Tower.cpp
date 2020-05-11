//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "Tower.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "Enemy.h"
#include "Bullet.h"

Tower::Tower(class Game* game):Actor(game)
{
	SpriteComponent* sc = new SpriteComponent(this, 200);
	sc->setTexture(game->renderer()->getTexture("Assets/Tower.png"));
	move_ = new MoveComponent(this);
	//move_->setAngularXSpeed(Math::Pi);
	nextAttack_ = attackTime_;
}
void Tower::updateActor(float deltaTime)
{
	Actor::updateActor(deltaTime);
	nextAttack_ -= deltaTime;
	if(nextAttack_ <= 0.0f)
	{
		Enemy* e = game()->getNearestEnemy(position());
		if(e != nullptr)
		{
			//Vector from me to enemy
			vector2 dir = e->position() - position();
			float dist = dir.Length();
			if(dist < attackRange_)
			{
				//Rotate to face enemy
				setRotation(Math::Atan2(-dir.y, dir.x));
				//Spawn bullet at tower position facing enemy
				Bullet* b = new Bullet(game());
				b->setPosition(position());
				b->setRotation(rotation());
			}
		}
		nextAttack_ += attackTime_;
	}
}