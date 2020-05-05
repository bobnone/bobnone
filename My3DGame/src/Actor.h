// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <vector>
#include "Math.h"
#include "Component.h"

class Actor
{
public:
	enum TypeID
	{
		TActor = 0,
		TFollowActor,
		TPlaneActor,
		TTargetActor,

		NUM_ACTOR_TYPES
	};
	static const char* TypeNames[NUM_ACTOR_TYPES];
	enum ActorState
	{
		ACTOR_ACTIVE,
		ACTOR_PAUSED,
		ACTOR_DEAD
	};
	Actor(class Game* game);
	virtual ~Actor();
	// Update function called from Game (not overridable)
	void Update(float deltaTime);
	// Updates all the components attached to the actor (not overridable)
	void UpdateComponents(float deltaTime);
	// Any actor-specific update code (overridable)
	virtual void UpdateActor(float deltaTime);
	// ProcessInput function called from Game (not overridable)
	void ProcessInput(const uint8_t* keyState);
	// Any actor-specific input code (overridable)
	virtual void ActorInput(const uint8_t* keyState);
	// Getters/setters
	const vector3& GetPosition() const
	{
		return mPosition;
	}
	void SetPosition(const vector3& pos)
	{
		mPosition = pos;
		mRecomputeTransform = true;
	}
	const vector3& GetVelocity() const
	{
		return mVelocity;
	}
	const vector3& GetAcceleration() const
	{
		return mAcceleration;
	}
	const bool IsMoving() const
	{
		return mMoving;
	}
	float GetScale() const
	{
		return mScale;
	}
	void SetScale(float scale)
	{
		mScale = scale;
		mRecomputeTransform = true;
	}
	const quaternion& GetRotation() const
	{
		return mRotation;
	}
	void SetRotation(const quaternion& rotation)
	{
		mRotation = rotation;
		mRecomputeTransform = true;
	}
	void ComputeWorldTransform();
	const matrix4& GetWorldTransform() const
	{
		return mWorldTransform;
	}
	vector3 GetRight() const
	{
		return vector3::Transform(vector3::UnitY, mRotation);
	}
	vector3 GetUp() const
	{
		return vector3::Transform(vector3::UnitZ, mRotation);
	}
	vector3 GetForward() const
	{
		return vector3::Transform(vector3::UnitX, mRotation);
	}
	void RotateToNewForward(const vector3& forward);
	ActorState GetState() const
	{
		return mState;
	}
	void SetState(ActorState state)
	{
		mState = state;
	}
	class Game* GetGame()
	{
		return mGame;
	}
	// Add/remove components
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);
	// Load/Save
	virtual void LoadProperties(const rapidjson::Value& inObj);
	virtual void SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const;
	// Create an actor with specified properties
	template <typename T>
	static Actor* Create(class Game* game, const rapidjson::Value& inObj)
	{
		// Dynamically allocate actor of type T
		T* t = new T(game);
		// Call LoadProperties on new actor
		t->LoadProperties(inObj);
		return t;
	}
	// Search through component vector for one of type
	Component* GetComponentOfType(Component::TypeID type)
	{
		Component* comp = nullptr;
		for (Component* c : mComponents)
		{
			if (c->GetType() == type)
			{
				comp = c;
				break;
			}
		}
		return comp;
	}
	virtual TypeID GetType() const
	{
		return TActor;
	}
	const std::vector<Component*>& GetComponents() const
	{
		return mComponents;
	}
protected:
	// Actor's state
	ActorState mState;
	// Transform
	matrix4 mWorldTransform;
	vector3 mPosition;
	vector3 mOldPosition;
	vector3 mVelocity;
	vector3 mOldVelocity;
	vector3 mAcceleration;
	quaternion mRotation;
	float mScale;
	bool mRecomputeTransform;
	bool mMoving;
	std::vector<Component*> mComponents;
	class Game* mGame;
};