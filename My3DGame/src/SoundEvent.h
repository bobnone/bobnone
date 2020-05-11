//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include <string>
#include "Math.h"
#include "Actor.h"

class SoundEvent
{
public:
	SoundEvent();
	//Returns true if associated FMOD event still exists
	bool isValid();
	//Restart event from beginning
	void restart();
	//Stop this event
	void stop(bool allowFadeOut = true);
	//Setters
	void setPaused(bool pause);
	void setVolume(float value);
	void setPitch(float value);
	void setParameter(const std::string& name, float value);
	//Getters
	bool getPaused() const;
	float getVolume() const;
	float getPitch() const;
	float getParameter(const std::string& name);
	//Positional
	bool is3D() const;
	void set3DAttributes(class Actor* owner);
protected:
	//Make this constructor protected and AudioSystem a friend
	//so that only AudioSystem can access this constructor.
	friend class AudioSystem;
	SoundEvent(class AudioSystem* system, unsigned int id);
private:
	class AudioSystem* system_;
	unsigned int ID_;
};