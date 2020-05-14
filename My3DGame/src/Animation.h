//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include "BoneTransform.h"
#include <vector>
#include <string>

class Animation
{
public:
	bool load(const std::string& fileName);
	size_t numBones() const
	{
		return numBones_;
	}
	size_t numFrames() const
	{
		return numFrames_;
	}
	float duration() const
	{
		return duration_;
	}
	float frameDuration() const
	{
		return frameDuration_;
	}
	//Fills the provided vector with the global (current) pose matrices for each
	//bone at the specified time in the animation. It is expected that the time
	const std::string& fileName() const
	{
		return fileName_;
	}
	//is >= 0.0f and <= mDuration
	void getGlobalPoseAtTime(std::vector<Matrix4>& outPoses, const class Skeleton* inSkeleton, float inTime) const;
private:
	//Number of bones for the animation
	size_t numBones_;
	//Number of frames in the animation
	size_t numFrames_;
	//Duration of the animation in seconds
	float duration_;
	//Duration of each frame in the animation
	float frameDuration_;
	//Transform information for each frame on the track
	//Each index in the outer vector is a bone, inner vector is a frame
	std::vector<std::vector<BoneTransform>> tracks_;
	std::string fileName_;
};