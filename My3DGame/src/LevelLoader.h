// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <string>
#include <rapidjson/document.h>
#include <functional>
#include <unordered_map>

using ActorFunc = std::function<class Actor*(class Game*, const rapidjson::Value&)>;
using ComponentFunc = std::function<class Component*(class Actor*, const rapidjson::Value&)>;

class LevelLoader
{
public:
	// Load the level -- returns true if successful
	static bool LoadLevel(class Game* game, const std::string& fileName);
	// Loads a JSON file into a RapidJSON document
	static bool LoadJSON(const std::string& fileName, rapidjson::Document& outDoc);
	// Save the level
	static void SaveLevel(class Game* game, const std::string& fileName);
protected:
	// Helper to load global properties
	static void LoadGlobalProperties(class Game* game, const rapidjson::Value& inObject);
	// Helper to load in actors
	static void LoadActors(class Game* game, const rapidjson::Value& inArray);
	// Helper to load in components
	static void LoadComponents(class Actor* actor, const rapidjson::Value& inArray);
	// Maps for data
	static std::unordered_map<std::string, ActorFunc> sActorFactoryMap;
	static std::unordered_map<std::string, std::pair<int, ComponentFunc>> sComponentFactoryMap;
	// Helper to save global properties
	static void SaveGlobalProperties(rapidjson::Document::AllocatorType& alloc, class Game* game, rapidjson::Value& inObject);
	// Helper to save actors
	static void SaveActors(rapidjson::Document::AllocatorType& alloc, class Game* game, rapidjson::Value& inArray);
	// Helper to save components
	static void SaveComponents(rapidjson::Document::AllocatorType& alloc, const class Actor* actor, rapidjson::Value& inArray);
};