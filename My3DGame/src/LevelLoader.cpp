//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "LevelLoader.h"
#include <fstream>
#include <SDL/SDL.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
#include "Game.h"
#include "Renderer.h"
#include "FollowActor.h"
#include "PlaneActor.h"
#include "TargetActor.h"
#include "AudioComponent.h"
#include "BoxComponent.h"
#include "CameraComponent.h"
#include "FollowCamera.h"
#include "MoveComponent.h"
#include "MeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "SpriteComponent.h"
#include "MirrorCamera.h"
#include "PointLightComponent.h"
#include "TargetComponent.h"
#include "JsonHelper.h"

const int LevelVersion = 1;

//Declare map of actors to spawn functions
std::unordered_map<std::string, ActorFunc> LevelLoader::actorFactoryMap_
{
	{ "Actor", &Actor::create<Actor> },
	{ "FollowActor", &Actor::create<FollowActor> },
	{ "PlaneActor", &Actor::create<PlaneActor> },
	{ "TargetActor", &Actor::create<TargetActor> },
};
std::unordered_map<std::string, std::pair<int, ComponentFunc>> LevelLoader::componentFactoryMap_
{
	{ "AudioComponent", { Component::TAudioComponent, &Component::create<AudioComponent>} },
	{ "BoxComponent", { Component::TBoxComponent, &Component::create<BoxComponent> } },
	{ "CameraComponent", { Component::TCameraComponent, &Component::create<CameraComponent> } },
	{ "FollowCamera", { Component::TFollowCamera, &Component::create<FollowCamera> } },
	{ "MeshComponent", { Component::TMeshComponent, &Component::create<MeshComponent> } },
	{ "MoveComponent", { Component::TMoveComponent, &Component::create<MoveComponent> } },
	{ "SkeletalMeshComponent", { Component::TSkeletalMeshComponent, &Component::create<SkeletalMeshComponent> } },
	{ "SpriteComponent", { Component::TSpriteComponent, &Component::create<SpriteComponent> } },
	{ "MirrorCamera", { Component::TMirrorCamera, &Component::create<MirrorCamera> } },
	{ "PointLightComponent", { Component::TPointLightComponent, &Component::create<PointLightComponent> }},
	{ "TargetComponent",{ Component::TTargetComponent, &Component::create<TargetComponent> } },
};
bool LevelLoader::loadLevel(Game* game, const std::string& fileName)
{
	rapidjson::Document doc;
	if(!loadJSON(fileName, doc))
	{
		SDL_Log("Failed to load level %s", fileName.c_str());
		return false;
	}
	int version = 0;
	if (!JsonHelper::getInt(doc, "version", version) || version != LevelVersion)
	{
		SDL_Log("Incorrect level file version for %s", fileName.c_str());
		return false;
	}
	//Handle any global properties
	const rapidjson::Value& globals = doc["globalProperties"];
	if (globals.IsObject())
	{
		loadGlobalProperties(game, globals);
	}
	//Handle any actors
	const rapidjson::Value& actors = doc["actors"];
	if(actors.IsArray())
	{
		loadActors(game, actors);
	}
	return true;
}
bool LevelLoader::loadJSON(const std::string& fileName, rapidjson::Document& outDoc)
{
	//Load the file from disk into an ifstream in binary mode, loaded with stream buffer at the end (ate)
	std::ifstream file(fileName, std::ios::in | std::ios::binary | std::ios::ate);
	if(!file.is_open())
	{
		SDL_Log("File %s not found", fileName.c_str());
		return false;
	}
	//Get the current position in stream buffer, which is size of file
	std::ifstream::pos_type fileSize = file.tellg();
	//Seek back to start of file
	file.seekg(0, std::ios::beg);
	//Create a vector of size + 1 (for null terminator)
	std::vector<char> bytes(static_cast<size_t>(fileSize) + 1);
	//Read in bytes into vector
	file.read(bytes.data(), static_cast<size_t>(fileSize));
	//Load raw data into RapidJSON document
	outDoc.Parse(bytes.data());
	if(!outDoc.IsObject())
	{
		SDL_Log("File %s is not valid JSON", fileName.c_str());
		return false;
	}
	return true;
}
void LevelLoader::saveLevel(Game* game, const std::string& fileName)
{
	//Create the document and root object
	rapidjson::Document doc;
	doc.SetObject();
	//Write the version
	JsonHelper::addInt(doc.GetAllocator(), doc, "version", LevelVersion);
	//Globals
	rapidjson::Value globals(rapidjson::kObjectType);
	saveGlobalProperties(doc.GetAllocator(), game, globals);
	doc.AddMember("globalProperties", globals, doc.GetAllocator());
	//Actors
	rapidjson::Value actors(rapidjson::kArrayType);
	saveActors(doc.GetAllocator(), game, actors);
	doc.AddMember("actors", actors, doc.GetAllocator());
	//Save JSON to string buffer
	rapidjson::StringBuffer buffer;
	//Use PrettyWriter for pretty output (otherwise use Writer)
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);
	const char* output = buffer.GetString();
	//Write output to file
	std::ofstream outFile(fileName);
	if (outFile.is_open())
	{
		outFile << output;
	}
}
void LevelLoader::loadGlobalProperties(Game* game, const rapidjson::Value& inObject)
{
	//Get ambient light
	Vector3 ambient;
	if (JsonHelper::getVector3(inObject, "ambientLight", ambient))
	{
		game->renderer()->setAmbientLight(ambient);
	}
	//Get directional light
	const rapidjson::Value& dirObj = inObject["directionalLight"];
	if(dirObj.IsObject())
	{
		DirectionalLight& light = game->renderer()->getDirectionalLight();
		//Set direction/color, if they exist
		JsonHelper::getVector3(dirObj, "direction", light.direction_);
		JsonHelper::getVector3(dirObj, "diffuseColor", light.diffuseColor_);
		JsonHelper::getVector3(dirObj, "specularColor", light.specularColor_);
	}
}
void LevelLoader::loadActors(Game* game, const rapidjson::Value& inArray)
{
	//Loop through array of actors
	for(rapidjson::SizeType i = 0; i < inArray.Size(); i++)
	{
		const rapidjson::Value& actorObj = inArray[i];
		if(actorObj.IsObject())
		{
			//Get the type
			std::string type;
			if (JsonHelper::getString(actorObj, "type", type))
			{
				//Is this type in the map?
				auto iter = actorFactoryMap_.find(type);
				if(iter != actorFactoryMap_.end())
				{
					//Construct with function stored in map
					Actor* actor = iter->second(game, actorObj["properties"]);
					//Get the actor's components
					if(actorObj.HasMember("components"))
					{
						const rapidjson::Value& components = actorObj["components"];
						if(components.IsArray())
						{
							loadComponents(actor, components);
						}
					}
				}
				else
				{
					SDL_Log("Unknown actor type %s", type.c_str());
				}
			}
		}
	}
}
void LevelLoader::loadComponents(Actor* actor, const rapidjson::Value& inArray)
{
	//Loop through array of components
	for(rapidjson::SizeType i = 0; i < inArray.Size(); i++)
	{
		const rapidjson::Value& compObj = inArray[i];
		if(compObj.IsObject())
		{
			//Get the type
			std::string type;
			if(JsonHelper::getString(compObj, "type", type))
			{
				auto iter = componentFactoryMap_.find(type);
				if(iter != componentFactoryMap_.end())
				{
					//Get the type-id of component
					Component::TypeID tid = static_cast<Component::TypeID>(iter->second.first);
					//Does the actor already have a component of this type?
					Component* comp = actor->getComponentOfType(tid);
					if(comp == nullptr)
					{
						//It's a new component, call function from map
						comp = iter->second.second(actor, compObj["properties"]);
					}
					else
					{
						//It already exists, just load properties
						comp->loadProperties(compObj["properties"]);
					}
				}
				else
				{
					SDL_Log("Unknown component type %s", type.c_str());
				}
			}
		}
	}
}
void LevelLoader::saveGlobalProperties(rapidjson::Document::AllocatorType& alloc, Game* game, rapidjson::Value& inObject)
{
	// Ambient light
	JsonHelper::addVector3(alloc, inObject, "ambientLight", game->renderer()->ambientLight());
	// Directional light
	DirectionalLight& dirLight = game->renderer()->getDirectionalLight();
	rapidjson::Value dirObj(rapidjson::kObjectType);
	JsonHelper::addVector3(alloc, dirObj, "direction", dirLight.direction_);
	JsonHelper::addVector3(alloc, dirObj, "diffuseColor", dirLight.diffuseColor_);
	JsonHelper::addVector3(alloc, dirObj, "specularColor", dirLight.specularColor_);
	inObject.AddMember("directionalLight", dirObj, alloc);
}
void LevelLoader::saveActors(rapidjson::Document::AllocatorType& alloc, Game* game, rapidjson::Value& inArray)
{
	const auto& actors = game->actors();
	for (const Actor* actor: actors)
	{
		//Make a JSON object
		rapidjson::Value obj(rapidjson::kObjectType);
		//Add type
		JsonHelper::addString(alloc, obj, "type", Actor::TypeNames[actor->getType()]);
		//Make object for properties
		rapidjson::Value props(rapidjson::kObjectType);
		//Save properties
		actor->saveProperties(alloc, props);
		//Add the properties member
		obj.AddMember("properties", props, alloc);
		// Save components
		rapidjson::Value components(rapidjson::kArrayType);
		saveComponents(alloc, actor, components);
		obj.AddMember("components", components, alloc);
		//Add actor to inArray
		inArray.PushBack(obj, alloc);
	}
}
void LevelLoader::saveComponents(rapidjson::Document::AllocatorType& alloc, const Actor* actor, rapidjson::Value& inArray)
{
	const auto& components = actor->components();
	for(const Component* comp : components)
	{
		//Make a JSON object
		rapidjson::Value obj(rapidjson::kObjectType);
		//Add type
		JsonHelper::addString(alloc, obj, "type", Component::TypeNames[comp->getType()]);
		//Make an object for properties
		rapidjson::Value props(rapidjson::kObjectType);
		//Save rest of properties
		comp->saveProperties(alloc, props);
		//Add the member
		obj.AddMember("properties", props, alloc);
		//Add component to array
		inArray.PushBack(obj, alloc);
	}
}