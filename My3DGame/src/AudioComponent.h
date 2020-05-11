//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include "Component.h"
#include <vector>
#include "SoundEvent.h"

class AudioComponent : public Component
{
public:
	AudioComponent(class Actor* owner, int updateOrder = 200);
	~AudioComponent();
	void update(float deltaTime) override;
	void onUpdateWorldTransform() override;
	SoundEvent playEvent(const std::string& name);
	void stopAllEvents();
	TypeID getType() const override
	{
		return TAudioComponent;
	}
private:
	std::vector<SoundEvent> events2D_;
	std::vector<SoundEvent> events3D_;
};