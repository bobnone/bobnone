//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "Enemy.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "NavComponent.h"
#include "Grid.h"
#include "Tile.h"
#include "CircleComponent.h"
#include <algorithm>

Enemy::Enemy(class Game* game):Actor(game)
{
	//Add to enemy vector
	game->getEnemies().emplace_back(this);
	SpriteComponent* sc = new SpriteComponent(this);
	sc->setTexture(game->renderer()->getTexture("Assets/Airplane.png"));
	//Set position at start tile
	setPosition(game->getGrid()->getStartTile()->getPosition());
	//Setup a nav component at the start tile
	NavComponent* nc = new NavComponent(this);
	nc->setForwardSpeed(150.0f);
	nc->startPath(game->getGrid()->getStartTile());
	//Setup a circle for collision
	circle_ = new CircleComponent(this);
	circle_->setRadius(25.0f);
}
Enemy::~Enemy()
{
	//Remove from enemy vector
	auto iter = std::find(game()->getEnemies().begin(), game()->getEnemies().end(), this);
	game()->getEnemies().erase(iter);
}
void Enemy::updateActor(float deltaTime)
{
	Actor::updateActor(deltaTime);
	//Am I near the end tile?
	vector2 diff = position() - game()->getGrid()->getEndTile()->getPosition();
	if(Math::NearZero(diff.Length(), 10.0f))
	{
		setState(ACTOR_DEAD);
	}
}