//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <GL/glew.h>
#include <SDL/SDL_types.h>
#include "SoundEvent.h"
#include "Actor.h"
#include "Renderer.h"
#include "Mouse.h"
#include "Physics.h"
#include "AudioSystem.h"
#include "HUD.h"
#include "UIScreen.h"
#include "FollowActor.h"
#include "Font.h"
#include "Skeleton.h"
#include "Animation.h"
#include "SpriteComponent.h"

class Game
{
public:
	enum GameState
	{
		GAME_PLAY,
		GAME_PAUSED,
		GAME_QUIT
	};
	Game();
	bool initialize();
	void runLoop();
	void shutdown();
	void addActor(class Actor* actor);
	void removeActor(class Actor* actor);
	class Renderer* renderer()
	{
		return renderer_;
	}
	class AudioSystem* audioSystem()
	{
		return audioSystem_;
	}
	class Physics* physics()
	{
		return physics_;
	}
	class Mouse* mouse()
	{
		return mouse_;
	}
	class HUD* getHUD()
	{
		return HUD_;
	}
	//Manage UI stack
	const std::vector<class UIScreen*>& UIStack()
	{
		return UIStack_;
	}
	void pushUI(class UIScreen* screen);
	class FollowActor* player()
	{
		return player_;
	}
	GameState getState() const
	{
		return gameState_;
	}
	void setState(GameState state)
	{
		gameState_ = state;
	}
	class Font* getFont(const std::string& fileName);
	void loadText(const std::string& fileName);
	const std::string& getText(const std::string& key);
	class Skeleton* getSkeleton(const std::string& fileName);
	class Animation* getAnimation(const std::string& fileName);
	const std::vector<class Actor*>& actors() const
	{
		return actors_;
	}
	void setPlayer(class FollowActor* actor)
	{
		player_ = actor;
	}
private:
	void processInput();
	void handleKeyPress(int key);
	void updateGame();
	void generateOutput();
	void loadData();
	void unloadData();
	//All the actors in the game
	std::vector<class Actor*> actors_;
	std::vector<class UIScreen*> UIStack_;
	//Map for fonts
	std::unordered_map<std::string, class Font*> fonts_;
	//Map of loaded skeletons
	std::unordered_map<std::string, class Skeleton*> skeletons_;
	//Map of loaded animations
	std::unordered_map<std::string, class Animation*> anims_;
	//Map for text localization
	std::unordered_map<std::string, std::string> text_;
	//Any pending actors
	std::vector<class Actor*> pendingActors_;
	class Renderer* renderer_;
	class AudioSystem* audioSystem_;
	class Physics* physics_;
	class Mouse* mouse_;
	class HUD* HUD_;
	Uint32 ticksCount_;
	GameState gameState_;
	//Track if we're updating actors right now
	bool updatingActors_;
	//Game-specific code
	class FollowActor* player_;
	class SpriteComponent* crosshair_;
	SoundEvent musicEvent_;
};