// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

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
	SetRotation(quaternion(vector3::UnitZ, Math::Pi));
	MeshComponent* mc = new MeshComponent(this);
	Renderer* renderer = GetGame()->GetRenderer();
	Mesh* mesh = renderer->GetMesh("Assets/Target.gpmesh");
	mc->SetMesh(mesh);
	mc->SetShader(renderer->GetShader("Mesh"));
	// Add collision box
	BoxComponent* bc = new BoxComponent(this);
	bc->SetObjectBox(mesh->GetBox());
	new TargetComponent(this);

}