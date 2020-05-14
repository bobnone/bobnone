//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "Animation.h"
#include "Skeleton.h"
#include <SDL/SDL_log.h>
#include "LevelLoader.h"

bool Animation::load(const std::string& fileName)
{
	fileName_ = fileName;
	rapidjson::Document doc;
	if(!LevelLoader::loadJSON(fileName, doc))
	{
		SDL_Log("Failed to load animation %s", fileName.c_str());
		return false;
	}
	int ver = doc["version"].GetInt();
	//Check the meta data
	if(ver != 1)
	{
		SDL_Log("Animation %s unknown format", fileName.c_str());
		return false;
	}
	const rapidjson::Value& sequence = doc["sequence"];
	if(!sequence.IsObject())
	{
		SDL_Log("Animation %s doesn't have a sequence.", fileName.c_str());
		return false;
	}
	const rapidjson::Value& frames = sequence["frames"];
	const rapidjson::Value& length = sequence["length"];
	const rapidjson::Value& bonecount = sequence["bonecount"];
	if(!frames.IsUint() || !length.IsDouble() || !bonecount.IsUint())
	{
		SDL_Log("Sequence %s has invalid frames, length, or bone count.", fileName.c_str());
		return false;
	}
	numFrames_ = frames.GetUint();
	duration_ = length.GetDouble();
	numBones_ = bonecount.GetUint();
	frameDuration_ = duration_ / (numFrames_ - 1);
	tracks_.resize(numBones_);
	const rapidjson::Value& tracks = sequence["tracks"];
	if(!tracks.IsArray())
	{
		SDL_Log("Sequence %s missing a tracks array.", fileName.c_str());
		return false;
	}
	for(rapidjson::SizeType i = 0; i < tracks.Size(); i++)
	{
		if(!tracks[i].IsObject())
		{
			SDL_Log("Animation %s: Track element %d is invalid.", fileName.c_str(), i);
			return false;
		}
		size_t boneIndex = tracks[i]["bone"].GetUint();
		const rapidjson::Value& transforms = tracks[i]["transforms"];
		if(!transforms.IsArray())
		{
			SDL_Log("Animation %s: Track element %d is missing transforms.", fileName.c_str(), i);
			return false;
		}
		BoneTransform temp;
		if(transforms.Size() < numFrames_)
		{
			SDL_Log("Animation %s: Track element %d has fewer frames than expected.", fileName.c_str(), i);
			return false;
		}
		for(rapidjson::SizeType j = 0; j < transforms.Size(); j++)
		{
			const rapidjson::Value& rot = transforms[j]["rot"];
			const rapidjson::Value& trans = transforms[j]["trans"];
			if(!rot.IsArray() || !trans.IsArray())
			{
				SDL_Log("Skeleton %s: Bone %d is invalid.", fileName.c_str(), i);
				return false;
			}
			temp.rotation_.x = rot[0].GetDouble();
			temp.rotation_.y = rot[1].GetDouble();
			temp.rotation_.z = rot[2].GetDouble();
			temp.rotation_.w = rot[3].GetDouble();
			temp.translation_.x = trans[0].GetDouble();
			temp.translation_.y = trans[1].GetDouble();
			temp.translation_.z = trans[2].GetDouble();
			tracks_[boneIndex].emplace_back(temp);
		}
	}
	return true;
}
void Animation::getGlobalPoseAtTime(std::vector<Matrix4>& outPoses, const Skeleton* inSkeleton, float inTime) const
{
	if(outPoses.size() != numBones_)
	{
		outPoses.resize(numBones_);
	}
	//Figure out the current frame index and next frame
	//Note: This assumes inTime is bounded by [0, AnimDuration]
	size_t frame = static_cast<size_t>(inTime / frameDuration_);
	size_t nextFrame = frame + 1;
	//Calculate fractional value between frame and next frame
	float pct = inTime / frameDuration_ - frame;
	//Setup the pose for the root
	if(tracks_[0].size() > 0)
	{
		//Interpolate between the current frame's pose and the next frame
		BoneTransform interp = BoneTransform::interpolate(tracks_[0][frame], tracks_[0][nextFrame], pct);
		outPoses[0] = interp.toMatrix();
	}
	else
	{
		outPoses[0] = Matrix4();
	}
	const std::vector<Skeleton::Bone>& bones = inSkeleton->bones();
	//Now setup the poses for the rest
	for(size_t bone = 1; bone < numBones_; bone++)
	{
		Matrix4 localMat; //(Defaults to identity)
		if (tracks_[bone].size() > 0)
		{
			BoneTransform interp = BoneTransform::interpolate(tracks_[bone][frame], tracks_[bone][nextFrame], pct);
			localMat = interp.toMatrix();
		}
		outPoses[bone] = localMat * outPoses[bones[bone].parent_];
	}
}