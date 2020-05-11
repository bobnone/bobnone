//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once

class AIState
{
public:
	AIState(class AIComponent* owner):owner_(owner)
	{
		//EMPTY:
	}
	//State-specific behavior
	virtual void update(float deltaTime) = 0;
	virtual void onEnter() = 0;
	virtual void onExit() = 0;
	//Getter for string name of state
	virtual const char* getName() const = 0;
protected:
	class AIComponent* owner_;
};

class AIPatrol : public AIState
{
public:
	AIPatrol(class AIComponent* owner):AIState(owner)
	{
		//EMPTY:
	}
	//Override with behaviors for this state
	void update(float deltaTime) override;
	void onEnter() override;
	void onExit() override;
	const char* getName() const override
	{
		return "Patrol";
	}
};

class AIDeath:public AIState
{
public:
	AIDeath(class AIComponent* owner):AIState(owner)
	{
		//EMPTY:
	}
	void update(float deltaTime) override;
	void onEnter() override;
	void onExit() override;
	const char* getName() const override
	{
		return "Death";
	}
};

class AIAttack:public AIState
{
public:
	AIAttack(class AIComponent* owner):AIState(owner)
	{
		//EMPTY:
	}
	void update(float deltaTime) override;
	void onEnter() override;
	void onExit() override;
	const char* getName() const override
	{
		return "Attack";
	}
};