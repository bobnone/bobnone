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
	Quaternion yaw(Vector3::UNIT_Z, yawSpeed_ * deltaTime);
	//Transform offset and up by yaw
	offset_ = Math::transform(offset_, yaw);
	up_ = Math::transform(up_, yaw);
	//Compute camera forward/right from these vectors
	//Forward owner.position - (owner.position + offset) = -offset
	Vector3 forward = -1.0f * offset_;
	forward.normalize();
	Vector3 right = Math::cross(up_, forward);
	right.normalize();
	//Create quaternion for pitch about camera right
	Quaternion pitch(right, pitchSpeed_ * deltaTime);
	//Transform camera offset and up by pitch
	offset_ = Math::transform(offset_, pitch);
	up_ = Math::transform(up_, pitch);
	//Compute transform matrix
	Vector3 target = owner_->position();
	Vector3 cameraPos = target + offset_;
	Matrix4x4 view = Math::lookAt(cameraPos, target, up_);
	setViewMatrix(view);
}