//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

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
	//Update function called from Game (not overridable)
	void update(float deltaTime);
	//Updates all the components attached to the actor (not overridable)
	void updateComponents(float deltaTime);
	//Any actor-specific update code (overridable)
	virtual void updateActor(float deltaTime);
	//ProcessInput function called from Game (not overridable)
	void processInput(const uint8_t* keyState);
	//Any actor-specific input code (overridable)
	virtual void actorInput(const uint8_t* keyState);
	//Getters/setters
	const vector3& position() const
	{
		return position_;
	}
	void setPosition(const vector3& position)
	{
		position_ = position;
		recomputeTransform_ = true;
	}
	const vector3& velocity() const
	{
		return velocity_;
	}
	const vector3& acceleration() const
	{
		return acceleration_;
	}
	const bool moving() const
	{
		return moving_;
	}
	float scale() const
	{
		return scale_;
	}
	void setScale(float scale)
	{
		scale_ = scale;
		recomputeTransform_ = true;
	}
	const quaternion& rotation() const
	{
		return rotation_;
	}
	void setRotation(const quaternion& rotation)
	{
		rotation_ = rotation;
		recomputeTransform_ = true;
	}
	void computeWorldTransform();
	const matrix4& worldTransform() const
	{
		return worldTransform_;
	}
	vector3 getRight() const
	{
		return vector3::Transform(vector3::UnitY, rotation_);
	}
	vector3 getUp() const
	{
		return vector3::Transform(vector3::UnitZ, rotation_);
	}
	vector3 getForward() const
	{
		return vector3::Transform(vector3::UnitX, rotation_);
	}
	void rotateToNewForward(const vector3& forward);
	ActorState state() const
	{
		return state_;
	}
	void setState(ActorState state)
	{
		state_ = state;
	}
	class Game* game()
	{
		return game_;
	}
	// Add/remove components
	void addComponent(class Component* component);
	void removeComponent(class Component* component);
	// Load/Save
	virtual void loadProperties(const rapidjson::Value& inObj);
	virtual void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const;
	// Create an actor with specified properties
	template <typename T>
	static Actor* create(class Game* game, const rapidjson::Value& inObj)
	{
		// Dynamically allocate actor of type T
		T* t = new T(game);
		// Call LoadProperties on new actor
		t->loadProperties(inObj);
		return t;
	}
	// Search through component vector for one of type
	Component* getComponentOfType(Component::TypeID type)
	{
		Component* comp = nullptr;
		for (Component* c : components_)
		{
			if (c->getType() == type)
			{
				comp = c;
				break;
			}
		}
		return comp;
	}
	virtual TypeID getType() const
	{
		return TActor;
	}
	const std::vector<Component*>& components() const
	{
		return components_;
	}
protected:
	//Actor's state
	ActorState state_;
	//Transform
	matrix4 worldTransform_;
	vector3 position_;
	vector3 oldPosition_;
	vector3 velocity_;
	vector3 oldVelocity_;
	vector3 acceleration_;
	quaternion rotation_;
	float scale_;
	bool recomputeTransform_;
	bool moving_;
	std::vector<Component*> components_;
	class Game* game_;
};