//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "OrbitCamera.h"
#include "Actor.h"

OrbitCamera::OrbitCamera(Actor* owner):CameraComponent(owner), offset_(-400.0f, 0.0f, 0.0f), up_(vector3::UnitZ), pitchSpeed_(0.0f), yawSpeed_(0.0f)
{
	//EMPTY:
}
void OrbitCamera::update(float deltaTime)
{
	CameraComponent::update(deltaTime);
	//Create a quaternion for yaw about world up
	quaternion yaw(vector3::UnitZ, yawSpeed_ * deltaTime);
	//Transform offset and up by yaw
	offset_ = vector3::Transform(offset_, yaw);
	up_ = vector3::Transform(up_, yaw);
	//Compute camera forward/right from these vectors
	//Forward owner.position - (owner.position + offset) = -offset
	vector3 forward = -1.0f * offset_;
	forward.Normalize();
	vector3 right = vector3::Cross(up_, forward);
	right.Normalize();
	//Create quaternion for pitch about camera right
	quaternion pitch(right, pitchSpeed_ * deltaTime);
	//Transform camera offset and up by pitch
	offset_ = vector3::Transform(offset_, pitch);
	up_ = vector3::Transform(up_, pitch);
	//Compute transform matrix
	vector3 target = owner_->position();
	vector3 cameraPos = target + offset_;
	matrix4 view = matrix4::CreateLookAt(cameraPos, target, up_);
	setViewMatrix(view);
}