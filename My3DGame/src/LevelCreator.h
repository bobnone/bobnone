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
#include "Math.h"

using ActorFunc = std::function<class Actor*(class Game*, const rapidjson::Value&)>;
using ComponentFunc = std::function<class Component*(class Actor*, const rapidjson::Value&)>;

class LevelCreator
{
public:
	// Creates a new level
	static void NewLevel(class Game* game, const std::string& fileName);
	// Unload current level data
	static void UnloadLevel(class Game* game);
protected:
	// Helper to create global properties
	static void NewGlobalProperties(rapidjson::Document::AllocatorType& alloc, class Game* game, rapidjson::Value& inObject);
	// Helpers to create actors
	static void CreateActors(class Game* game);
	static void NewActors(rapidjson::Document::AllocatorType& alloc, class Game* game, rapidjson::Value& inArray);
	// Helper to save components
	static void SaveComponents(rapidjson::Document::AllocatorType& alloc, const class Actor* actor, rapidjson::Value& inArray);
};