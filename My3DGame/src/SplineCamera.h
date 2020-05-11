//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include "CameraComponent.h"
#include <vector>

struct Spline
{
	//Control points for spline
	//(Requires n+2 points where n is number
	//of points in segment)
	std::vector<vector3> controlPoints_;
	//Given spline segment where startIdx = P1,
	//compute position based on t value
	vector3 compute(size_t startIdx, float t) const;
	//Returns number of control points
	size_t getNumPoints() const
	{
		return controlPoints_.size();
	}
};

class SplineCamera: public CameraComponent
{
public:
	SplineCamera(class Actor* owner);
	void update(float deltaTime) override;
	//Restart the spline
	void restart();
	void setSpeed(float speed)
	{
		speed_ = speed;
	}
	void setSpline(const Spline& spline)
	{
		path_ = spline;
	}
	void setPaused(bool pause)
	{
		paused_ = pause;
	}
private:
	//Spline path camera follows
	Spline path_;
	//Current control point index and t
	size_t index_;
	float T_;
	//Amount t changes/sec
	float speed_;
	//Whether to move the camera long the path
	bool paused_;
};