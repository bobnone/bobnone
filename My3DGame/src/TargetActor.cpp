//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "TargetActor.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "BoxComponent.h"
#include "Mesh.h"
#include "Shader.h"
#include "TargetComponent.h"

TargetActor::TargetActor(Game* game):Actor(game)
{
	setRotation(Quaternion(Vector3::UNIT_Z, Math::PI));
	MeshComponent* mc = new MeshComponent(this);
	Mesh* mesh = game->renderer()->getMesh("Assets/Target.gpmesh");
	mc->setMesh(mesh);
	mc->setShader(game->renderer()->getShader("Mesh"));
	//Add collision box
	BoxComponent* bc = new BoxComponent(this);
	bc->setObjectBox(mesh->box());
	new TargetComponent(this);
}