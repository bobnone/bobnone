//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "FPSCamera.h"
#include "Actor.h"

FPSCamera::FPSCamera(Actor* owner):CameraComponent(owner), pitchSpeed_(0.0f), maxPitch_(Math::Pi / 3.0f), pitch_(0.0f)
{
	//EMPTY:
}
void FPSCamera::update(float deltaTime)
{
	//Call parent update (doesn't do anything right now)
	CameraComponent::update(deltaTime);
	//Camera position is owner position
	vector3 cameraPos = owner_->position();
	//Update pitch based on pitch speed
	pitch_ += pitchSpeed_ * deltaTime;
	//Clamp pitch to [-max, +max]
	pitch_ = Math::Clamp(pitch_, -maxPitch_, maxPitch_);
	//Make a quaternion representing pitch rotation,
	//which is about owner's right vector
	quaternion q(owner_->getRight(), pitch_);
	//Rotate owner forward by pitch quaternion
	vector3 viewForward = vector3::Transform(owner_->getForward(), q);
	//Target position 100 units in front of view forward
	vector3 target = cameraPos + viewForward * 100.0f;
	//Also rotate up by pitch quaternion
	vector3 up = vector3::Transform(vector3::UnitZ, q);
	//Create look at matrix, set as view
	matrix4 view = matrix4::CreateLookAt(cameraPos, target, up);
	setViewMatrix(view);
}