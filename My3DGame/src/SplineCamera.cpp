//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "SplineCamera.h"
#include "Actor.h"

Vector3 Spline::compute(size_t startIdx, float t) const
{
	//Check if startIdx is out of bounds
	if(startIdx >= controlPoints_.size())
	{
		return controlPoints_.back();
	}
	else if(startIdx == 0)
	{
		return controlPoints_[startIdx];
	}
	else if(startIdx + 2 >= controlPoints_.size())
	{
		return controlPoints_[startIdx];
	}
	//Get p0 through p3
	Vector3 p0 = controlPoints_[startIdx - 1];
	Vector3 p1 = controlPoints_[startIdx];
	Vector3 p2 = controlPoints_[startIdx + 1];
	Vector3 p3 = controlPoints_[startIdx + 2];
	//Compute position according to Catmull-Rom equation
	Vector3 position = 0.5f * ((2.0f * p1) + (-1.0f * p0 + p2) * t + (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * t * t + (-1.0f * p0 + 3.0f * p1 - 3.0f * p2 + p3) * t * t * t);
	return position;
}
SplineCamera::SplineCamera(Actor* owner):CameraComponent(owner), index_(1), T_(0.0f), speed_(0.5f), paused_(true)
{
	//EMPTY:
}
void SplineCamera::update(float deltaTime)
{
	CameraComponent::update(deltaTime);
	//Update t value
	if(!paused_)
	{
		T_ += speed_ * deltaTime;
		//Advance to the next control point if needed.
		//This assumes speed isn't so fast that you jump past
		//multiple control points in one frame.
		if(T_ >= 1.0f)
		{
			//Make sure we have enough points to advance the path
			if(index_ < path_.getNumPoints() - 3)
			{
				index_++;
				T_ = T_ - 1.0f;
			}
			else
			{
				//Path's done, so pause
				paused_ = true;
			}
		}
	}
	//Camera position is the spline at the current t/index
	Vector3 cameraPos = path_.compute(index_, T_);
	//Target point is just a small delta ahead on the spline
	Vector3 target = path_.compute(index_, T_ + 0.01f);
	//Assume spline doesn't flip upside-down
	const Vector3 up = Vector3::UNIT_Z;
	Matrix4x4 view = Math::lookAt(cameraPos, target, up);
	setViewMatrix(view);
}
void SplineCamera::restart()
{
	index_ = 1;
	T_ = 0.0f;
	paused_ = false;
}