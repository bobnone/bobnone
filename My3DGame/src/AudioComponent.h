// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Component.h"
#include <vector>
#include "SoundEvent.h"

class AudioComponent : public Component
{
public:
	AudioComponent(class Actor* owner, int updateOrder = 200);
	~AudioComponent();
	void Update(float deltaTime) override;
	void OnUpdateWorldTransform() override;
	SoundEvent PlayEvent(const std::string& name);
	void StopAllEvents();
	TypeID GetType() const override
	{
		return TAudioComponent;
	}
private:
	std::vector<SoundEvent> mEvents2D;
	std::vector<SoundEvent> mEvents3D;
};