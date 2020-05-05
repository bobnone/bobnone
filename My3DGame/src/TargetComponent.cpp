// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "TargetComponent.h"
#include "Actor.h"
#include "Game.h"
#include "HUD.h"

TargetComponent::TargetComponent(Actor * owner) :Component(owner)
{
	mOwner->GetGame()->GetHUD()->AddTargetComponent(this);
}
TargetComponent::~TargetComponent()
{
	mOwner->GetGame()->GetHUD()->RemoveTargetComponent(this);
}