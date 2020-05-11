//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "AIComponent.h"
#include "Actor.h"
#include "AIState.h"
#include <SDL/SDL_log.h>

AIComponent::AIComponent(class Actor* owner):Component(owner),currentState_(nullptr)
{
	//EMPTY:
}
void AIComponent::update(float deltaTime)
{
	if(currentState_)
	{
		currentState_->update(deltaTime);
	}
}
void AIComponent::changeState(const std::string& name)
{
	//First exit the current state
	if(currentState_)
	{
		currentState_->onExit();
	}
	//Try to find the new state from the map
	auto iter = stateMap_.find(name);
	if (iter != stateMap_.end())
	{
		currentState_ = iter->second;
		//We're entering the new state
		currentState_->onEnter();
	}
	else
	{
		SDL_Log("Could not find AIState %s in state map", name.c_str());
		currentState_ = nullptr;
	}
}
void AIComponent::registerState(AIState* state)
{
	stateMap_.emplace(state->getName(), state);
}