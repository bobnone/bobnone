// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Game.h"
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "PauseMenu.h"
#include "LevelLoader.h"
#include "LevelCreator.h"

Game::Game():renderer_(nullptr), audioSystem_(nullptr), physics_(nullptr), HUD_(nullptr), mouse_(nullptr), ticksCount_(0), gameState_(GAME_PLAY), updatingActors_(false), player_(nullptr), crosshair_(nullptr)
{
	//EMPTY:
}
bool Game::initialize()
{
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	//Create the renderer
	renderer_ = new Renderer(this);
	if(!renderer_->initialize(1024.0f, 768.0f))
	{
		SDL_Log("Failed to initialize renderer");
		delete renderer_;
		renderer_ = nullptr;
		return false;
	}
	//Create the audio system
	audioSystem_ = new AudioSystem(this);
	if (!audioSystem_->initialize())
	{
		SDL_Log("Failed to initialize audio system");
		audioSystem_->shutdown();
		delete audioSystem_;
		audioSystem_ = nullptr;
		return false;
	}
	//Create the physics world
	physics_ = new Physics(this);
	//Initialize SDL_ttf
	if(TTF_Init() != 0)
	{
		SDL_Log("Failed to initialize SDL_ttf");
		return false;
	}
	loadData();
	ticksCount_ = SDL_GetTicks();
	return true;
}
void Game::runLoop()
{
	while(gameState_ != GAME_QUIT)
	{
		processInput();
		updateGame();
		generateOutput();
	}
}
void Game::processInput()
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		mouse_->processInput(&event);
		switch(event.type)
		{
		case SDL_QUIT:
			gameState_ = GAME_QUIT;
			break;
			//This fires when a key's initially pressed
		case SDL_KEYDOWN:
			if(!event.key.repeat)
			{
				if(gameState_ == GAME_PLAY)
				{
					handleKeyPress(event.key.keysym.sym);
				}
				else if(!UIStack_.empty())
				{
					UIStack_.back()->handleKeyPress(event.key.keysym.sym);
				}
			}
			break;
		case SDL_KEYUP:
			//EMPTY:
			break;
		default:
			break;
		}
	}
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if(gameState_ == GAME_PLAY)
	{
		for(auto actor : actors_)
		{
			if(actor->state() == Actor::ACTOR_ACTIVE)
			{
				actor->processInput(state);
			}
		}
	}
	else if(!UIStack_.empty())
	{
		UIStack_.back()->processInput(state);
	}
}
void Game::handleKeyPress(int key)
{
	switch(key)
	{
	case SDLK_ESCAPE:
		//Create pause menu
		new PauseMenu(this);
		break;
	case '-':
	{
		//Reduce master volume
		float volume = audioSystem_->getBusVolume("bus:/");
		volume = Math::Max(0.0f, volume - 0.1f);
		audioSystem_->setBusVolume("bus:/", volume);
		break;
	}
	case '=':
	{
		//Increase master volume
		float volume = audioSystem_->getBusVolume("bus:/");
		volume = Math::Min(1.0f, volume + 0.1f);
		audioSystem_->setBusVolume("bus:/", volume);
		break;
	}
	case '1':
	{
		//Load English text
		loadText("Assets/English.gptext");
		break;
	}
	case '2':
	{
		//Load Russian text
		loadText("Assets/Russian.gptext");
		break;
	}
	case 'f':
	{
		//Load level
		LevelCreator::unloadLevel(this);
		//LevelLoader::loadLevel(this, "Assets/Saved.gplevel");
		LevelLoader::loadLevel(this, "Assets/Level4.gplevel");
		break;
	}
	case 'r':
	{
		//Save level
		LevelLoader::saveLevel(this, "Assets/Saved.gplevel");
		break;
	}
	default:
		break;
	}
}
void Game::updateGame()
{
	//Compute delta time
	const int delay = 16;
	//Wait until 16ms has elapsed since last frame
	while(!SDL_TICKS_PASSED(SDL_GetTicks(), ticksCount_ + delay))
	{
		//EMPTY:
	}
	float deltaTime = (SDL_GetTicks() - ticksCount_) / 1000.0f;
	if(deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	ticksCount_ = SDL_GetTicks();
	//Update the mouse
	mouse_->update(deltaTime);
	if(gameState_ == GAME_PLAY)
	{
		//Update all actors
		updatingActors_ = true;
		for(auto actor : actors_)
		{
			actor->update(deltaTime);
		}
		updatingActors_ = false;
		//Move any pending actors to mActors
		for(auto pending : pendingActors_)
		{
			pending->computeWorldTransform();
			actors_.emplace_back(pending);
		}
		pendingActors_.clear();
		//Add any dead actors to a temp vector
		std::vector<Actor*> deadActors;
		for(auto actor : actors_)
		{
			if(actor->state() == Actor::ACTOR_DEAD)
			{
				deadActors.emplace_back(actor);
			}
		}
		//Delete dead actors (which removes them from mActors)
		for(auto actor : deadActors)
		{
			delete actor;
		}
	}
	//Update audio system
	audioSystem_->update(deltaTime);
	//Update UI screens
	for(auto ui : UIStack_)
	{
		if(ui->state() == UIScreen::UI_ACTIVE)
		{
			ui->update(deltaTime);
		}
	}
	//Delete any UIScreens that are closed
	auto iter = UIStack_.begin();
	while(iter != UIStack_.end())
	{
		if((*iter)->state() == UIScreen::UI_CLOSING)
		{
			delete *iter;
			iter = UIStack_.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}
void Game::generateOutput()
{
	renderer_->draw();
}
void Game::loadData()
{
	//Load English text
	loadText("Assets/English.gptext");
	//Create Mouse
	mouse_ = new Mouse(this);
	//Create HUD
	HUD_ = new HUD(this);
	//Load the level from file
	LevelLoader::loadLevel(this, "Assets/Level4.gplevel");
	//LevelCreator::newLevel(this, "Assets/NewLevel.gplevel");
	//Start music
	musicEvent_ = audioSystem_->playEvent("event:/Music");
	//Enable relative mouse mode for camera look
	mouse_->setRelative(true);
}
void Game::unloadData()
{
	//Delete actors
	//Because ~Actor calls RemoveActor, have to use a different style loop
	while(!actors_.empty())
	{
		delete actors_.back();
	}
	//Clear the UI stack
	while(!UIStack_.empty())
	{
		delete UIStack_.back();
		UIStack_.pop_back();
	}
	if(renderer_)
	{
		renderer_->unloadData();
	}
	//Unload fonts
	for(auto f : fonts_)
	{
		f.second->unload();
		delete f.second;
	}
	//Unload skeletons
	for(auto s : skeletons_)
	{
		delete s.second;
	}
	//Unload animations
	for (auto a : anims_)
	{
		delete a.second;
	}
}
void Game::shutdown()
{
	unloadData();
	TTF_Quit();
	delete physics_;
	if (renderer_)
	{
		renderer_->shutdown();
	}
	if (audioSystem_)
	{
		audioSystem_->shutdown();
	}
	SDL_Quit();
}
void Game::addActor(Actor* actor)
{
	//If we're updating actors, need to add to pending
	if(updatingActors_)
	{
		pendingActors_.emplace_back(actor);
	}
	else
	{
		actors_.emplace_back(actor);
	}
}
void Game::removeActor(Actor* actor)
{
	//Is it in pending actors?
	auto iter = std::find(pendingActors_.begin(), pendingActors_.end(), actor);
	if (iter != pendingActors_.end())
	{
		//Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, pendingActors_.end() - 1);
		pendingActors_.pop_back();
	}
	//Is it in actors?
	iter = std::find(actors_.begin(), actors_.end(), actor);
	if(iter != actors_.end())
	{
		//Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, actors_.end() - 1);
		actors_.pop_back();
	}
}
void Game::pushUI(UIScreen* screen)
{
	UIStack_.emplace_back(screen);
}
Font* Game::getFont(const std::string& fileName)
{
	auto iter = fonts_.find(fileName);
	if(iter != fonts_.end())
	{
		return iter->second;
	}
	else
	{
		Font* font = new Font(this);
		if (font->load(fileName))
		{
			fonts_.emplace(fileName, font);
		}
		else
		{
			font->unload();
			delete font;
			font = nullptr;
		}
		return font;
	}
}
void Game::loadText(const std::string& fileName)
{
	//Clear the existing map, if already loaded
	text_.clear();
	rapidjson::Document doc;
	if (!LevelLoader::loadJSON(fileName, doc))
	{
		SDL_Log("Failed to load text file %s", fileName.c_str());
		return;
	}
	//Parse the text map
	const rapidjson::Value& actions = doc["TextMap"];
	for(rapidjson::Value::ConstMemberIterator i = actions.MemberBegin(); i != actions.MemberEnd(); ++i)
	{
		if(i->name.IsString() && i->value.IsString())
		{
			text_.emplace(i->name.GetString(), i->value.GetString());
		}
	}
}
const std::string& Game::getText(const std::string& key)
{
	static std::string errorMsg("**KEY NOT FOUND**");
	// Find this text in the map, if it exists
	auto iter = text_.find(key);
	if(iter != text_.end())
	{
		return iter->second;
	}
	else
	{
		return errorMsg;
	}
}
Skeleton* Game::getSkeleton(const std::string& fileName)
{
	auto iter = skeletons_.find(fileName);
	if(iter != skeletons_.end())
	{
		return iter->second;
	}
	else
	{
		Skeleton* sk = new Skeleton();
		if (sk->load(fileName))
		{
			skeletons_.emplace(fileName, sk);
		}
		else
		{
			delete sk;
			sk = nullptr;
		}
		return sk;
	}
}
Animation* Game::getAnimation(const std::string& fileName)
{
	auto iter = anims_.find(fileName);
	if (iter != anims_.end())
	{
		return iter->second;
	}
	else
	{
		Animation* anim = new Animation();
		if (anim->load(fileName))
		{
			anims_.emplace(fileName, anim);
		}
		else
		{
			delete anim;
			anim = nullptr;
		}
		return anim;
	}
}