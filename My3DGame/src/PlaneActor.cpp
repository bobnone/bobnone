// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "PlaneActor.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "BoxComponent.h"
#include "Mesh.h"

PlaneActor::PlaneActor(Game* game) :Actor(game)
{
	SetScale(10.0f);
	MeshComponent* mc = new MeshComponent(this);
	Renderer* renderer = GetGame()->GetRenderer();
	Mesh* mesh = renderer->GetMesh("Assets/Plane.gpmesh");
	mc->SetMesh(mesh);
	mc->SetShader(renderer->GetShader("Mesh"));
	// Add collision box
	BoxComponent* bc = new BoxComponent(this);
	bc->SetObjectBox(mesh->GetBox());
}