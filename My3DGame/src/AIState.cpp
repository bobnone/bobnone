//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "AIState.h"
#include "AIComponent.h"
#include <SDL/SDL_log.h>

void AIPatrol::update(float deltaTime)
{
	SDL_Log("Updating %s state", getName());
	bool dead = true;
	if(dead)
	{
		owner_->changeState("Death");
	}
}
void AIPatrol::onEnter()
{
	SDL_Log("Entering %s state", getName());
}
void AIPatrol::onExit()
{
	SDL_Log("Exiting %s state", getName());
}
void AIDeath::update(float deltaTime)
{
	SDL_Log("Updating %s state", getName());
}
void AIDeath::onEnter()
{
	SDL_Log("Entering %s state", getName());
}
void AIDeath::onExit()
{
	SDL_Log("Exiting %s state", getName());
}
void AIAttack::update(float deltaTime)
{
	SDL_Log("Updating %s state", getName());
}
void AIAttack::onEnter()
{
	SDL_Log("Entering %s state", getName());
}
void AIAttack::onExit()
{
	SDL_Log("Exiting %s state", getName());
}