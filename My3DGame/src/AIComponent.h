//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include "Component.h"
#include <unordered_map>
#include <string>

class AIComponent : public Component
{
public:
	AIComponent(class Actor* owner);
	void update(float deltaTime) override;
	void changeState(const std::string& name);
	//Add a new state to the map
	void registerState(class AIState* state);
private:
	//Maps name of state to AIState instance
	std::unordered_map<std::string, class AIState*> stateMap_;
	//Current state we're in
	class AIState* currentState_;
};