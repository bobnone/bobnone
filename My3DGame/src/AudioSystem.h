// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <unordered_map>
#include <string>
#include "Math.h"
#include "SoundEvent.h"

// Forward declarations to avoid including FMOD header
namespace FMOD
{
	class System;
	namespace Studio
	{
		class Bank;
		class EventDescription;
		class EventInstance;
		class System;
		class Bus;
	};
};
class AudioSystem
{
public:
	AudioSystem(class Game* game);
	~AudioSystem();
	bool initialize();
	void shutdown();
	// Load/unload banks
	void loadBank(const std::string& name);
	void unloadBank(const std::string& name);
	void unloadAllBanks();
	SoundEvent playEvent(const std::string& name);
	void update(float deltaTime);
	// For positional audio
	void setListener(class Actor* mOwner, const matrix4& viewMatrix);
	/* Used to manually set the position
	Note: Can be used to set virtual positions*/
	void AudioSystem::setListener(class Actor* mOwner, const matrix4& viewMatrix, const vector3 position);
	// Control buses
	float getBusVolume(const std::string& name) const;
	bool getBusPaused(const std::string& name) const;
	void setBusVolume(const std::string& name, float volume);
	void setBusPaused(const std::string& name, bool pause);
protected:
	friend class SoundEvent;
	FMOD::Studio::EventInstance* getEventInstance(unsigned int id);
private:
	// Tracks the next ID to use for event instances
	static unsigned int nextID_;
	class Game* game_;
	// Map of loaded banks
	std::unordered_map<std::string, FMOD::Studio::Bank*> banks_;
	// Map of event name to EventDescription
	std::unordered_map<std::string, FMOD::Studio::EventDescription*> events_;
	// Map of event id to EventInstance
	std::unordered_map<unsigned int, FMOD::Studio::EventInstance*> eventInstances_;
	// Map of buses
	std::unordered_map<std::string, FMOD::Studio::Bus*> buses_;
	// FMOD studio system
	FMOD::Studio::System* system_;
	// FMOD Low-level system (in case needed)
	FMOD::System* lowLevelSystem_;
};