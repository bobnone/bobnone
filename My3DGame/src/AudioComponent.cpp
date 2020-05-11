//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "AudioComponent.h"
#include "Actor.h"
#include "Game.h"
#include "AudioSystem.h"

AudioComponent::AudioComponent(Actor* owner, int updateOrder) :Component(owner, updateOrder)
{
	//EMPTY:
}
AudioComponent::~AudioComponent()
{
	stopAllEvents();
}
void AudioComponent::update(float deltaTime)
{
	Component::update(deltaTime);
	//Remove invalid 2D events
	auto iter = events2D_.begin();
	while(iter != events2D_.end())
	{
		if (!iter->isValid())
		{
			iter = events2D_.erase(iter);
		}
		else
		{
			++iter;
		}
	}
	//Remove invalid 3D events
	iter = events3D_.begin();
	while(iter != events3D_.end())
	{
		if(!iter->isValid())
		{
			iter = events3D_.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}
void AudioComponent::onUpdateWorldTransform()
{
	//Update 3D events' world transforms
	for(auto& event : events3D_)
	{
		if(event.isValid())
		{
			event.set3DAttributes(owner_);
		}
	}
}
SoundEvent AudioComponent::playEvent(const std::string& name)
{
	SoundEvent e = owner_->game()->audioSystem()->playEvent(name);
	//Is this 2D or 3D?
	if(e.is3D())
	{
		events3D_.emplace_back(e);
		//Set initial 3D attributes
		e.set3DAttributes(owner_);
	}
	else
	{
		events2D_.emplace_back(e);
	}
	return e;
}
void AudioComponent::stopAllEvents()
{
	//Stop all sounds
	for(auto& e : events2D_)
	{
		e.stop();
	}
	for(auto& e : events3D_)
	{
		e.stop();
	}
	//Clear events
	events2D_.clear();
	events3D_.clear();
}