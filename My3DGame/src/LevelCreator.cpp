#include "LevelCreator.h"
#include <fstream>
#include "JsonHelper.h"
#include "Game.h"
#include "Renderer.h"
#include "FollowActor.h"
#include "PlaneActor.h"
#include "TargetActor.h"
#include "PointLightComponent.h"
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>

const int LevelVersion = 1;

void LevelCreator::newLevel(Game* game, const std::string& fileName)
{
	//Create the document and root object
	rapidjson::Document doc;
	doc.SetObject();
	//Write the version
	JsonHelper::addInt(doc.GetAllocator(), doc, "version", LevelVersion);
	//Globals
	rapidjson::Value globals(rapidjson::kObjectType);
	newGlobalProperties(doc.GetAllocator(), game, globals);
	doc.AddMember("globalProperties", globals, doc.GetAllocator());
	//Actors
	rapidjson::Value actors(rapidjson::kArrayType);
	newActors(doc.GetAllocator(), game, actors);
	doc.AddMember("actors", actors, doc.GetAllocator());
	//Save JSON to string buffer
	rapidjson::StringBuffer buffer;
	//Use PrettyWriter for pretty output (otherwise use Writer)
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);
	const char* output = buffer.GetString();
	//Write output to file
	std::ofstream outFile(fileName);
	if(outFile.is_open())
	{
		outFile << output;
	}
}
void LevelCreator::unloadLevel(class Game* game)
{
	/*//Delete actors
	//Because ~Actor calls RemoveActor, have to use a different style loop
	//actors_ = game->getActors().empty();
	while(!actors_.empty())
	{
		delete actors_.back();
	}
	//Clear the UI stack
	while(!uIStack_.empty())
	{
		delete uIStack_.back();
		uIStack_.pop_back();
	}
	if(renderer_)
	{
		renderer_->unloadData();
	}
	//Unload fonts
	for(auto f: mFonts)
	{
		f.second->unload();
		delete f.second;
	}
	//Unload skeletons
	for(auto s: skeletons_)
	{
		delete s.second;
	}
	//Unload animations
	for(auto a: anims_)
	{
		delete a.second;
	}
	TTF_Quit();
	delete physWorld_;
	if(renderer_)
	{
		renderer_->shutdown();
	}
	if(audioSystem_)
	{
		audioSystem_->shutdown();
	}*/
}
void LevelCreator::newGlobalProperties(rapidjson::Document::AllocatorType& alloc, Game* game, rapidjson::Value& inObject)
{
	//Ambient light
	vector3 ambientLight = vector3(0.3f, 0.3f, 0.3f);
	game->renderer()->setAmbientLight(ambientLight);
	JsonHelper::addVector3(alloc, inObject, "ambientLight", ambientLight);
	//Directional light
	rapidjson::Value dirObj(rapidjson::kObjectType);
	DirectionalLight& light = game->renderer()->getDirectionalLight();
	light.direction_ = vector3(-1.0, -1.0, -10.0);
	light.diffuseColor_ = vector3(0.5, 0.5, 0.5);
	light.specularColor_ = vector3(0.5, 0.5, 0.5);
	JsonHelper::addVector3(alloc, dirObj, "direction", light.direction_);
	JsonHelper::addVector3(alloc, dirObj, "diffuseColor", light.diffuseColor_);
	JsonHelper::addVector3(alloc, dirObj, "specularColor", light.specularColor_);
	inObject.AddMember("directionalLight", dirObj, alloc);
}
void LevelCreator::createActors(Game* game)
{
	FollowActor* player = new FollowActor(game);
	PlaneActor* levelFloor0 = new PlaneActor(game);
	levelFloor0->setPosition(vector3(0.0, 0.0, -100.0));
	PlaneActor* levelFloor1 = new PlaneActor(game);
	levelFloor1->setPosition(vector3(1000.0, 0.0, -100.0));
	PlaneActor* levelFloor2 = new PlaneActor(game);
	levelFloor2->setPosition(vector3(2000.0, 0.0, -100.0));
	TargetActor* target0 = new TargetActor(game);
	target0->setPosition(vector3(1000.0, 0.0, 0.0));
	target0->setScale(10.0);
	Actor* posLight0 = new Actor(game);
	PointLightComponent* plc0 = new PointLightComponent(posLight0);
	plc0->diffuseColor_ = vector3(1.0, 0.0, 0.0);
	plc0->specularColor_ = vector3(0.5, 0.5, 0.5);
	plc0->innerRadius_ = 100;
	plc0->outerRadius_ = 200;
	Actor* posLight1 = new Actor(game);
	posLight1->setPosition(vector3(500.0, 500.0, 0.0));
	PointLightComponent* plc1 = new PointLightComponent(posLight1);
	plc1->diffuseColor_ = vector3(0.0, 1.0, 0.0);
	plc1->specularColor_ = vector3(0.5, 0.5, 0.5);
	plc1->innerRadius_ = 100;
	plc1->outerRadius_ = 200;
	Actor* posLight3 = new Actor(game);
	posLight3->setPosition(vector3(500.0, 0.0, 0.0));
	PointLightComponent* plc3 = new PointLightComponent(posLight3);
	plc3->diffuseColor_ = vector3(0.5, 0.5, 0.0);
	plc3->specularColor_ = vector3(0.5, 0.5, 0.5);
	plc3->innerRadius_ = 100;
	plc3->outerRadius_ = 200;
}
void LevelCreator::newActors(rapidjson::Document::AllocatorType& alloc, Game* game, rapidjson::Value& inArray)
{
	createActors(game);
	const auto& actors = game->actors();
	for(const Actor* actor: actors)
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
		//Save components
		rapidjson::Value components(rapidjson::kArrayType);
		saveComponents(alloc, actor, components);
		obj.AddMember("components", components, alloc);
		//Add actor to inArray
		inArray.PushBack(obj, alloc);
	}
}
void LevelCreator::saveComponents(rapidjson::Document::AllocatorType& alloc, const Actor* actor, rapidjson::Value& inArray)
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