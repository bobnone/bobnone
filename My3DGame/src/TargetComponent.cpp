//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "TargetComponent.h"
#include "Actor.h"
#include "Game.h"
#include "HUD.h"

TargetComponent::TargetComponent(Actor* owner) :Component(owner)
{
	owner_->game()->getHUD()->addTargetComponent(this);
}
TargetComponent::~TargetComponent()
{
	owner_->game()->getHUD()->removeTargetComponent(this);
}