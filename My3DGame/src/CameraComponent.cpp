//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "CameraComponent.h"
#include "Actor.h"
#include "Renderer.h"
#include "Game.h"
#include "AudioSystem.h"

CameraComponent::CameraComponent(Actor* owner, int updateOrder):Component(owner, updateOrder)
{
	//EMPTY:
}
void CameraComponent::setViewMatrix(const Matrix4x4& view)
{
	//Pass view matrix to renderer and audio system
	owner_->game()->renderer()->setViewMatrix(view);
	owner_->game()->audioSystem()->setListener(owner_, view);
}