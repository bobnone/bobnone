//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "Asteroid.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "Random.h"
#include "CircleComponent.h"

Asteroid::Asteroid(Game* game):Actor(game),circle_(nullptr)
{
	// Initialize to random position/orientation
	Vector2 randPos = Random::getVector(Vector2(-512.0f, -384.0f), Vector2(512.0f, 384.0f));
	setPosition(randPos);
	setRotation(Random::getFloatRange(0.0f, Math::TWO_PI));
	// Create a sprite component
	SpriteComponent* sc = new SpriteComponent(this);
	sc->setTexture(game->renderer()->getTexture("Assets/Asteroid.png"));
	// Create a move component, and set a forward speed
	MoveComponent* mc = new MoveComponent(this);
	mc->setForwardSpeed(150.0f);
	// Create a circle component (for collision)
	circle_ = new CircleComponent(this);
	circle_->setRadius(40.0f);
	// Add to mAsteroids in game
	game->addAsteroid(this);
}
Asteroid::~Asteroid()
{
	game()->removeAsteroid(this);
}