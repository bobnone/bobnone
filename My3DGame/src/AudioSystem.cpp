//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "AudioSystem.h"
#include <SDL/SDL_log.h>
#include <FMOD/fmod_studio.hpp>
#include <FMOD/fmod_errors.h>

unsigned int AudioSystem::nextID_ = 0;

AudioSystem::AudioSystem(Game* game) :game_(game), system_(nullptr), lowLevelSystem_(nullptr)
{
	//EMPTY:
}
AudioSystem::~AudioSystem()
{
	//EMPTY:
}
bool AudioSystem::initialize()
{
	//Initialize debug logging
	FMOD::Debug_Initialize(FMOD_DEBUG_LEVEL_ERROR/*Log only errors*/, FMOD_DEBUG_MODE_TTY/*Output to stdout*/);
	//Create FMOD studio system object
	FMOD_RESULT result;
	result = FMOD::Studio::System::create(&system_);
	if (result != FMOD_OK)
	{
		SDL_Log("Failed to create FMOD system: %s", FMOD_ErrorString(result));
		return false;
	}
	// Initialize FMOD studio system
	result = system_->initialize(512/*Max number of concurrent sounds*/, FMOD_STUDIO_INIT_NORMAL/*Use default settings*/, FMOD_INIT_NORMAL/*Use default settings*/, nullptr /*Usually null*/);
	if (result != FMOD_OK)
	{
		SDL_Log("Failed to initialize FMOD system: %s", FMOD_ErrorString(result));
		return false;
	}
	//Save the low-level system pointer
	//system_->getLowLevelSystem(&lowLevelSystem_);
	system_->getCoreSystem(&lowLevelSystem_);
	//Load the master banks (strings first)
	loadBank("Assets/Master Bank.strings.bank");
	loadBank("Assets/Master Bank.bank");
	return true;
}
void AudioSystem::shutdown()
{
	//Unload all banks
	unloadAllBanks();
	//Shutdown FMOD system
	if(system_)
	{
		system_->release();
	}
}
void AudioSystem::loadBank(const std::string& name)
{
	//Prevent double-loading
	if(banks_.find(name) != banks_.end())
	{
		return;
	}
	//Try to load bank
	FMOD::Studio::Bank* bank = nullptr;
	FMOD_RESULT result = system_->loadBankFile(name.c_str()/*File name of bank*/, FMOD_STUDIO_LOAD_BANK_NORMAL/*Normal loading*/, &bank/*Save pointer to bank*/);
	const int maxPathLength = 512;
	if (result == FMOD_OK)
	{
		//Add bank to map
		banks_.emplace(name, bank);
		//Load all non-streaming sample data
		bank->loadSampleData();
		//Get the number of events in this bank
		int numEvents = 0;
		bank->getEventCount(&numEvents);
		if(numEvents > 0)
		{
			//Get list of event descriptions in this bank
			std::vector<FMOD::Studio::EventDescription*> events(numEvents);
			bank->getEventList(events.data(), numEvents, &numEvents);
			char eventName[maxPathLength];
			for(int i = 0; i < numEvents; i++)
			{
				FMOD::Studio::EventDescription* e = events[i];
				//Get the path of this event (like event:/Explosion2D)
				e->getPath(eventName, maxPathLength, nullptr);
				//Add to event map
				events_.emplace(eventName, e);
			}
		}
		//Get the number of buses in this bank
		int numBuses = 0;
		bank->getBusCount(&numBuses);
		if(numBuses > 0)
		{
			//Get list of buses in this bank
			std::vector<FMOD::Studio::Bus*> buses(numBuses);
			bank->getBusList(buses.data(), numBuses, &numBuses);
			char busName[512];
			for(int i = 0; i < numBuses; i++)
			{
				FMOD::Studio::Bus* bus = buses[i];
				//Get the path of this bus (like bus:/SFX)
				bus->getPath(busName, 512, nullptr);
				//Add to buses map
				buses_.emplace(busName, bus);
			}
		}
	}
}
void AudioSystem::unloadBank(const std::string& name)
{
	//Ignore if not loaded
	auto iter = banks_.find(name);
	if(iter == banks_.end())
	{
		return;
	}
	//First we need to remove all events from this bank
	FMOD::Studio::Bank* bank = iter->second;
	int numEvents = 0;
	bank->getEventCount(&numEvents);
	if(numEvents > 0)
	{
		//Get event descriptions for this bank
		std::vector<FMOD::Studio::EventDescription*> events(numEvents);
		//Get list of events
		bank->getEventList(events.data(), numEvents, &numEvents);
		char eventName[512];
		for(int i = 0; i < numEvents; i++)
		{
			FMOD::Studio::EventDescription* e = events[i];
			//Get the path of this event
			e->getPath(eventName, 512, nullptr);
			//Remove this event
			auto eventi = events_.find(eventName);
			if(eventi != events_.end())
			{
				events_.erase(eventi);
			}
		}
	}
	//Get the number of buses in this bank
	int numBuses = 0;
	bank->getBusCount(&numBuses);
	if(numBuses > 0)
	{
		//Get list of buses in this bank
		std::vector<FMOD::Studio::Bus*> buses(numBuses);
		bank->getBusList(buses.data(), numBuses, &numBuses);
		char busName[512];
		for (int i = 0; i < numBuses; i++)
		{
			FMOD::Studio::Bus* bus = buses[i];
			//Get the path of this bus (like bus:/SFX)
			bus->getPath(busName, 512, nullptr);
			//Remove this bus
			auto busi = buses_.find(busName);
			if(busi != buses_.end())
			{
				buses_.erase(busi);
			}
		}
	}
	//Unload sample data and bank
	bank->unloadSampleData();
	bank->unload();
	//Remove from banks map
	banks_.erase(iter);
}
void AudioSystem::unloadAllBanks()
{
	for(auto& iter : banks_)
	{
		//Unload the sample data, then the bank itself
		iter.second->unloadSampleData();
		iter.second->unload();
	}
	banks_.clear();
	//No banks means no events
	events_.clear();
}
SoundEvent AudioSystem::playEvent(const std::string& name)
{
	unsigned int retID = 0;
	auto iter = events_.find(name);
	if(iter != events_.end())
	{
		//Create instance of event
		FMOD::Studio::EventInstance* event = nullptr;
		iter->second->createInstance(&event);
		if(event)
		{
			//Start the event instance
			event->start();
			//Get the next id, and add to map
			nextID_++;
			retID = nextID_;
			eventInstances_.emplace(retID, event);
		}
	}
	return SoundEvent(this, retID);
}
void AudioSystem::update(float deltaTime)
{
	//Find any stopped event instances
	std::vector<unsigned int> done;
	for(auto& iter: eventInstances_)
	{
		FMOD::Studio::EventInstance* e = iter.second;
		//Get the state of this event
		FMOD_STUDIO_PLAYBACK_STATE state;
		e->getPlaybackState(&state);
		if(state == FMOD_STUDIO_PLAYBACK_STOPPED)
		{
			//Release the event and add id to done
			e->release();
			done.emplace_back(iter.first);
		}
	}
	//Remove done event instances from map
	for(auto id : done)
	{
		eventInstances_.erase(id);
	}
	//Update FMOD
	system_->update();
}
namespace
{
	FMOD_VECTOR VecToFMOD(const Vector3& in)
	{
		//FMOD_VECTOR (+x right, +y up, +z forward)
		FMOD_VECTOR v;
		v.x = in.x;
		v.y = in.y;
		v.z = in.z;
		return v;
	}
}
void AudioSystem::setListener(class Actor* owner, const Matrix4x4& viewMatrix)
{
	//Invert the view matrix to get the correct vectors
	Matrix4x4 invView = viewMatrix;
	invView.invert();
	FMOD_3D_ATTRIBUTES listener;
	//Set position, forward, up
	listener.position = VecToFMOD(invView.getTranslation());
	//In the inverted view, third row is forward
	listener.forward = VecToFMOD(invView.getZAxis());
	//In the inverted view, second row is up
	listener.up = VecToFMOD(invView.getYAxis());
	//Set velocity
	listener.velocity = VecToFMOD(owner->velocity());
	//Send to FMOD
	system_->setListenerAttributes(0, &listener);
}
void AudioSystem::setListener(class Actor* owner, const Matrix4x4& viewMatrix, const Vector3 position)
{
	//Invert the view matrix to get the correct vectors
	Matrix4x4 invView = viewMatrix;
	invView.invert();
	FMOD_3D_ATTRIBUTES listener;
	//Set position, forward, up
	listener.position = VecToFMOD(position);
	//In the inverted view, third row is forward
	listener.forward = VecToFMOD(invView.getZAxis());
	//In the inverted view, second row is up
	listener.up = VecToFMOD(invView.getYAxis());
	//Set velocity
	listener.velocity = VecToFMOD(owner->velocity());
	//Send to FMOD
	system_->setListenerAttributes(0, &listener);
}
float AudioSystem::getBusVolume(const std::string& name) const
{
	float retVal = 0.0f;
	const auto iter = buses_.find(name);
	if(iter != buses_.end())
	{
		iter->second->getVolume(&retVal);
	}
	return retVal;
}
bool AudioSystem::getBusPaused(const std::string & name) const
{
	bool retVal = false;
	const auto iter = buses_.find(name);
	if(iter != buses_.end())
	{
		iter->second->getPaused(&retVal);
	}
	return retVal;
}
void AudioSystem::setBusVolume(const std::string& name, float volume)
{
	auto iter = buses_.find(name);
	if(iter != buses_.end())
	{
		iter->second->setVolume(volume);
	}
}
void AudioSystem::setBusPaused(const std::string & name, bool pause)
{
	auto iter = buses_.find(name);
	if(iter != buses_.end())
	{
		iter->second->setPaused(pause);
	}
}
FMOD::Studio::EventInstance* AudioSystem::getEventInstance(unsigned int id)
{
	FMOD::Studio::EventInstance* event = nullptr;
	auto iter = eventInstances_.find(id);
	if(iter != eventInstances_.end())
	{
		event = iter->second;
	}
	return event;
}