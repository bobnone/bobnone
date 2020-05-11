#pragma once
#include <string>
#include <rapidjson/document.h>

class LevelCreator
{
public:
	//Creates a new level
	static void newLevel(class Game* game, const std::string& fileName);
	//Unload current level data
	static void unloadLevel(class Game* game);
protected:
	//Helper to create global properties
	static void newGlobalProperties(rapidjson::Document::AllocatorType& alloc, class Game* game, rapidjson::Value& inObject);
	//Helpers to create actors
	static void createActors(class Game* game);
	static void newActors(rapidjson::Document::AllocatorType& alloc, class Game* game, rapidjson::Value& inArray);
	//Helper to save components
	static void saveComponents(rapidjson::Document::AllocatorType& alloc, const class Actor* actor, rapidjson::Value& inArray);
};