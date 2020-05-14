//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "SoundEvent.h"
#include "AudioSystem.h"
#include <FMOD/fmod_studio.hpp>

SoundEvent::SoundEvent(class AudioSystem* system, unsigned int id): system_(system), ID_(id)
{
	//EMPTY:
}
SoundEvent::SoundEvent(): system_(nullptr), ID_(0)
{
	//EMPTY:
}
bool SoundEvent::isValid()
{
	return (system_ && system_->getEventInstance(ID_) != nullptr);
}
void SoundEvent::restart()
{
	auto event = system_ ? system_->getEventInstance(ID_) : nullptr;
	if(event)
	{
		event->start();
	}
}
void SoundEvent::stop(bool allowFadeOut /* true */)
{
	auto event = system_ ? system_->getEventInstance(ID_) : nullptr;
	if(event)
	{
		FMOD_STUDIO_STOP_MODE mode = allowFadeOut ? FMOD_STUDIO_STOP_ALLOWFADEOUT:FMOD_STUDIO_STOP_IMMEDIATE;
		event->stop(mode);
	}
}
void SoundEvent::setPaused(bool pause)
{
	auto event = system_ ? system_->getEventInstance(ID_) : nullptr;
	if(event)
	{
		event->setPaused(pause);
	}
}
void SoundEvent::setVolume(float value)
{
	auto event = system_ ? system_->getEventInstance(ID_) : nullptr;
	if(event)
	{
		event->setVolume(value);
	}
}
void SoundEvent::setPitch(float value)
{
	auto event = system_ ? system_->getEventInstance(ID_) : nullptr;
	if(event)
	{
		event->setPitch(value);
	}
}
void SoundEvent::setParameter(const std::string& name, float value)
{
	auto event = system_ ? system_->getEventInstance(ID_): nullptr;
	if(event)
	{
		//event->setParameterValue(name.c_str(), value);
		event->setParameterByName(name.c_str(), value);
	}
}
bool SoundEvent::getPaused() const
{
	bool retVal = false;
	auto event = system_ ? system_->getEventInstance(ID_):nullptr;
	if(event)
	{
		event->getPaused(&retVal);
	}
	return retVal;
}
float SoundEvent::getVolume() const
{
	float retVal = 0.0f;
	auto event = system_ ? system_->getEventInstance(ID_) : nullptr;
	if(event)
	{
		event->getVolume(&retVal);
	}
	return retVal;
}
float SoundEvent::getPitch() const
{
	float retVal = 0.0f;
	auto event = system_ ? system_->getEventInstance(ID_) : nullptr;
	if(event)
	{
		event->getPitch(&retVal);
	}
	return retVal;
}
float SoundEvent::getParameter(const std::string& name)
{
	float retVal = 0.0f;
	auto event = system_ ? system_->getEventInstance(ID_) : nullptr;
	if(event)
	{
		//event->getParameterValue(name.c_str(), &retVal);
		event->getParameterByName(name.c_str(), &retVal);
	}
	return retVal;
}
bool SoundEvent::is3D() const
{
	bool retVal = false;
	auto event = system_ ? system_->getEventInstance(ID_) : nullptr;
	if(event)
	{
		//Get the event description
		FMOD::Studio::EventDescription* ed = nullptr;
		event->getDescription(&ed);
		if(ed)
		{
			ed->is3D(&retVal);
		}
	}
	return retVal;
}
namespace
{
	FMOD_VECTOR vecToFMOD(const Vector3& in)
	{
		//FMOD_VECTOR(+x right, +y up, +z forward)
		FMOD_VECTOR v;
		v.x = in.x;
		v.y = in.y;
		v.z = in.z;
		return v;
	}
}
void SoundEvent::set3DAttributes(Actor* owner)
{
	auto event = system_ ? system_->getEventInstance(ID_):nullptr;
	if(event)
	{
		FMOD_3D_ATTRIBUTES attr;
		const Matrix4 worldTrans = owner->worldTransform();
		//Set position, forward, up
		attr.position = vecToFMOD(worldTrans.getTranslation());
		//In world transform, first row is forward
		attr.forward = vecToFMOD(worldTrans.getXAxis());
		//Third row is up
		attr.up = vecToFMOD(worldTrans.getZAxis());
		//Set velocity
		attr.velocity = vecToFMOD(owner->velocity());
		event->set3DAttributes(&attr);
	}
}