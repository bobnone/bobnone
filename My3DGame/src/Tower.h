//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include "Actor.h"

class Tower:public Actor
{
public:
	Tower(class Game* game);
	void updateActor(float deltaTime) override;
private:
	class MoveComponent* move_;
	float nextAttack_;
	const float attackTime_ = 2.5f;
	const float attackRange_ = 100.0f;
};