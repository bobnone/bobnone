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

void LevelCreator::NewLevel(Game* game, const std::string& fileName)
{
	// Create the document and root object
	rapidjson::Document doc;
	doc.SetObject();
	// Write the version
	JsonHelper::AddInt(doc.GetAllocator(), doc, "version", LevelVersion);
	// Globals
	rapidjson::Value globals(rapidjson::kObjectType);
	NewGlobalProperties(doc.GetAllocator(), game, globals);
	doc.AddMember("globalProperties", globals, doc.GetAllocator());
	// Actors
	rapidjson::Value actors(rapidjson::kArrayType);
	NewActors(doc.GetAllocator(), game, actors);
	doc.AddMember("actors", actors, doc.GetAllocator());
	// Save JSON to string buffer
	rapidjson::StringBuffer buffer;
	// Use PrettyWriter for pretty output (otherwise use Writer)
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);
	const char* output = buffer.GetString();
	// Write output to file
	std::ofstream outFile(fileName);
	if (outFile.is_open())
	{
		outFile << output;
	}
}
void LevelCreator::UnloadLevel(class Game* game)
{
	/*// Delete actors
	// Because ~Actor calls RemoveActor, have to use a different style loop
	//mActors = game->GetActors().empty();
	while (!mActors.empty())
	{
		delete mActors.back();
	}
	// Clear the UI stack
	while (!mUIStack.empty())
	{
		delete mUIStack.back();
		mUIStack.pop_back();
	}
	if (mRenderer)
	{
		mRenderer->UnloadData();
	}
	// Unload fonts
	for (auto f : mFonts)
	{
		f.second->Unload();
		delete f.second;
	}
	// Unload skeletons
	for (auto s : mSkeletons)
	{
		delete s.second;
	}
	// Unload animations
	for (auto a : mAnims)
	{
		delete a.second;
	}
	TTF_Quit();
	delete mPhysWorld;
	if (mRenderer)
	{
		mRenderer->Shutdown();
	}
	if (mAudioSystem)
	{
		mAudioSystem->Shutdown();
	}*/
}
void LevelCreator::NewGlobalProperties(rapidjson::Document::AllocatorType& alloc, Game* game, rapidjson::Value& inObject)
{
	// Ambient light
	vector3 ambientLight = vector3(0.3f, 0.3f, 0.3f);
	game->GetRenderer()->SetAmbientLight(ambientLight);
	JsonHelper::AddVector3(alloc, inObject, "ambientLight", ambientLight);
	// Directional light
	rapidjson::Value dirObj(rapidjson::kObjectType);
	DirectionalLight& light = game->GetRenderer()->GetDirectionalLight();
	light.mDirection = vector3(-1.0, -1.0, -10.0);
	light.mDiffuseColor = vector3(0.5, 0.5, 0.5);
	light.mSpecularColor = vector3(0.5, 0.5, 0.5);
	JsonHelper::AddVector3(alloc, dirObj, "direction", light.mDirection);
	JsonHelper::AddVector3(alloc, dirObj, "diffuseColor", light.mDiffuseColor);
	JsonHelper::AddVector3(alloc, dirObj, "specularColor", light.mSpecularColor);
	inObject.AddMember("directionalLight", dirObj, alloc);
}
void LevelCreator::CreateActors(Game* game)
{
	FollowActor* player = new FollowActor(game);
	PlaneActor* levelFloor0 = new PlaneActor(game);
	levelFloor0->SetPosition(vector3(0.0, 0.0, -100.0));
	PlaneActor* levelFloor1 = new PlaneActor(game);
	levelFloor1->SetPosition(vector3(1000.0, 0.0, -100.0));
	PlaneActor* levelFloor2 = new PlaneActor(game);
	levelFloor2->SetPosition(vector3(2000.0, 0.0, -100.0));
	TargetActor* target0 = new TargetActor(game);
	target0->SetPosition(vector3(1000.0, 0.0, 0.0));
	target0->SetScale(10.0);
	Actor* posLight0 = new Actor(game);
	PointLightComponent* plc0 = new PointLightComponent(posLight0);
	plc0->mDiffuseColor = vector3(1.0, 0.0, 0.0);
	plc0->mSpecularColor = vector3(0.5, 0.5, 0.5);
	plc0->mInnerRadius = 100;
	plc0->mOuterRadius = 200;
	Actor* posLight1 = new Actor(game);
	posLight1->SetPosition(vector3(500.0, 500.0, 0.0));
	PointLightComponent* plc1 = new PointLightComponent(posLight1);
	plc1->mDiffuseColor = vector3(0.0, 1.0, 0.0);
	plc1->mSpecularColor = vector3(0.5, 0.5, 0.5);
	plc1->mInnerRadius = 100;
	plc1->mOuterRadius = 200;
	Actor* posLight3 = new Actor(game);
	posLight3->SetPosition(vector3(500.0, 0.0, 0.0));
	PointLightComponent* plc3 = new PointLightComponent(posLight3);
	plc3->mDiffuseColor = vector3(0.5, 0.5, 0.0);
	plc3->mSpecularColor = vector3(0.5, 0.5, 0.5);
	plc3->mInnerRadius = 100;
	plc3->mOuterRadius = 200;
}
void LevelCreator::NewActors(rapidjson::Document::AllocatorType& alloc, Game* game, rapidjson::Value& inArray)
{
	CreateActors(game);
	const auto& actors = game->GetActors();
	for (const Actor* actor : actors)
	{
		// Make a JSON object
		rapidjson::Value obj(rapidjson::kObjectType);
		// Add type
		JsonHelper::AddString(alloc, obj, "type", Actor::TypeNames[actor->GetType()]);
		// Make object for properties
		rapidjson::Value props(rapidjson::kObjectType);
		// Save properties
		actor->SaveProperties(alloc, props);
		// Add the properties member
		obj.AddMember("properties", props, alloc);
		// Save components
		rapidjson::Value components(rapidjson::kArrayType);
		SaveComponents(alloc, actor, components);
		obj.AddMember("components", components, alloc);
		// Add actor to inArray
		inArray.PushBack(obj, alloc);
	}
}
void LevelCreator::SaveComponents(rapidjson::Document::AllocatorType& alloc, const Actor* actor, rapidjson::Value& inArray)
{
	const auto& components = actor->GetComponents();
	for (const Component* comp : components)
	{
		// Make a JSON object
		rapidjson::Value obj(rapidjson::kObjectType);
		// Add type
		JsonHelper::AddString(alloc, obj, "type", Component::TypeNames[comp->GetType()]);
		// Make an object for properties
		rapidjson::Value props(rapidjson::kObjectType);
		// Save rest of properties
		comp->SaveProperties(alloc, props);
		// Add the member
		obj.AddMember("properties", props, alloc);
		// Add component to array
		inArray.PushBack(obj, alloc);
	}
}