//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include "BoneTransform.h"
#include <string>
#include <vector>

class Skeleton
{
public:
	//Definition for each bone in the skeleton
	struct Bone
	{
		BoneTransform localBindPose_;
		std::string name_;
		int parent_;
	};
	//Load from a file
	bool load(const std::string& fileName);
	//Getter functions
	size_t getNumBones() const
	{
		return bones_.size();
	}
	const Bone& getBone(size_t idx) const
	{
		return bones_[idx];
	}
	const std::vector<Bone>& bones() const
	{
		return bones_;
	}
	const std::vector<Matrix4x4>& globalInvBindPoses() const
	{
		return globalInvBindPoses_;
	}
	const std::string& fileName() const
	{
		return fileName_;
	}
protected:
	//Called automatically when the skeleton is loaded
	//Computes the global inverse bind pose for each bone
	void computeGlobalInvBindPose();
private:
	//The bones in the skeleton
	std::vector<Bone> bones_;
	//The global inverse bind poses for each bone
	std::vector<Matrix4x4> globalInvBindPoses_;
	std::string fileName_;
};