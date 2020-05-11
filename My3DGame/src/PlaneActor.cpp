//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "PlaneActor.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "BoxComponent.h"
#include "Mesh.h"
#include "Shader.h"

PlaneActor::PlaneActor(Game* game):Actor(game)
{
	setScale(10.0f);
	MeshComponent* mc = new MeshComponent(this);
	Renderer* renderer = game->renderer();
	Mesh* mesh = renderer->getMesh("Assets/Plane.gpmesh");
	mc->setMesh(mesh);
	mc->setShader(renderer->getShader("Mesh"));
	//Add collision box
	BoxComponent* bc = new BoxComponent(this);
	bc->setObjectBox(mesh->box());
}