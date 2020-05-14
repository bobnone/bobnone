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
	Vector3 cameraPos = owner_->position();
	//Update pitch based on pitch speed
	pitch_ += pitchSpeed_ * deltaTime;
	//Clamp pitch to [-max, +max]
	pitch_ = Math::clamp(pitch_, -maxPitch_, maxPitch_);
	//Make a quaternion representing pitch rotation,
	//which is about owner's right vector
	Quaternion q(owner_->getRight(), pitch_);
	//Rotate owner forward by pitch quaternion
	Vector3 viewForward = Math::transform(owner_->getForward(), q);
	//Target position 100 units in front of view forward
	Vector3 target = cameraPos + viewForward * 100.0f;
	//Also rotate up by pitch quaternion
	Vector3 up = Math::transform(Vector3::UNIT_Z, q);
	//Create look at matrix, set as view
	Matrix4x4 view = Math::lookAt(cameraPos, target, up);
	setViewMatrix(view);
}