#pragma once
#include <string>
#include <rapidjson/document.h>

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