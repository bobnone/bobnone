//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include "Actor.h"

class BallActor : public Actor
{
public:
	BallActor(class Game* game);
	void updateActor(float deltaTime) override;
	void hitTarget();
	void loadProperties(const rapidjson::Value& inObj) override;
	void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const override;
	TypeID getType() const override
	{
		return TBallActor;
	}
private:
	class AudioComponent* audioComp_;
	float lifeSpan_;
};