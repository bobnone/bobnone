//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "Skeleton.h"
#include <SDL/SDL_log.h>
#include "MatrixPalette.h"
#include "LevelLoader.h"

bool Skeleton::load(const std::string& fileName)
{
	fileName_ = fileName;
	rapidjson::Document doc;
	if(!LevelLoader::loadJSON(fileName, doc))
	{
		SDL_Log("Failed to load skeleton %s", fileName.c_str());
		return false;
	}
	int ver = doc["version"].GetInt();
	//Check the meta data
	if(ver != 1)
	{
		SDL_Log("Skeleton %s unknown format", fileName.c_str());
		return false;
	}
	const rapidjson::Value& bonecount = doc["bonecount"];
	if (!bonecount.IsUint())
	{
		SDL_Log("Skeleton %s doesn't have a bone count.", fileName.c_str());
		return false;
	}
	size_t count = bonecount.GetUint();
	if(count > MAX_SKELETON_BONES)
	{
		SDL_Log("Skeleton %s exceeds maximum bone count.", fileName.c_str());
		return false;
	}
	bones_.reserve(count);
	const rapidjson::Value& bones = doc["bones"];
	if(!bones.IsArray())
	{
		SDL_Log("Skeleton %s doesn't have a bone array?", fileName.c_str());
		return false;
	}
	if(bones.Size() != count)
	{
		SDL_Log("Skeleton %s has a mismatch between the bone count and number of bones", fileName.c_str());
		return false;
	}
	Bone temp;
	for(rapidjson::SizeType i = 0; i < count; i++)
	{
		if(!bones[i].IsObject())
		{
			SDL_Log("Skeleton %s: Bone %d is invalid.", fileName.c_str(), i);
			return false;
		}
		const rapidjson::Value& name = bones[i]["name"];
		temp.name_ = name.GetString();
		const rapidjson::Value& parent = bones[i]["parent"];
		temp.parent_ = parent.GetInt();
		const rapidjson::Value& bindpose = bones[i]["bindpose"];
		if(!bindpose.IsObject())
		{
			SDL_Log("Skeleton %s: Bone %d is invalid.", fileName.c_str(), i);
			return false;
		}
		const rapidjson::Value& rot = bindpose["rot"];
		const rapidjson::Value& trans = bindpose["trans"];
		if(!rot.IsArray() || !trans.IsArray())
		{
			SDL_Log("Skeleton %s: Bone %d is invalid.", fileName.c_str(), i);
			return false;
		}
		temp.localBindPose_.rotation_.x = rot[0].GetDouble();
		temp.localBindPose_.rotation_.y = rot[1].GetDouble();
		temp.localBindPose_.rotation_.z = rot[2].GetDouble();
		temp.localBindPose_.rotation_.w = rot[3].GetDouble();
		temp.localBindPose_.translation_.x = trans[0].GetDouble();
		temp.localBindPose_.translation_.y = trans[1].GetDouble();
		temp.localBindPose_.translation_.z = trans[2].GetDouble();
		bones_.emplace_back(temp);
	}
	//Now that we have the bones
	computeGlobalInvBindPose();
	return true;
}
void Skeleton::computeGlobalInvBindPose()
{
	//Resize to number of bones, which automatically fills identity
	globalInvBindPoses_.resize(getNumBones());
	//Step 1: Compute global bind pose for each bone
	//The global bind pose for root is just the local bind pose
	globalInvBindPoses_[0] = bones_[0].localBindPose_.toMatrix();
	//Each remaining bone's global bind pose is its local pose
	//multiplied by the parent's global bind pose
	for(size_t i = 1; i < globalInvBindPoses_.size(); i++)
	{
		matrix4 localMat = bones_[i].localBindPose_.toMatrix();
		globalInvBindPoses_[i] = localMat * globalInvBindPoses_[bones_[i].parent_];
	}
	//Step 2: Invert
	for(size_t i = 0; i < globalInvBindPoses_.size(); i++)
	{
		globalInvBindPoses_[i].Invert();
	}
}