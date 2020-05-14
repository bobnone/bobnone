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
	const Vector3& position() const
	{
		return position_;
	}
	void setPosition(const Vector3& position)
	{
		position_ = position;
		recomputeTransform_ = true;
	}
	const Vector3& velocity() const
	{
		return velocity_;
	}
	const Vector3& acceleration() const
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
	const Quaternion& rotation() const
	{
		return rotation_;
	}
	void setRotation(const Quaternion& rotation)
	{
		rotation_ = rotation;
		recomputeTransform_ = true;
	}
	void computeWorldTransform();
	const Matrix4& worldTransform() const
	{
		return worldTransform_;
	}
	Vector3 getRight() const
	{
		return Vector3::transform(Vector3::UNIT_Y, rotation_);
	}
	Vector3 getUp() const
	{
		return Vector3::transform(Vector3::UNIT_Z, rotation_);
	}
	Vector3 getForward() const
	{
		return Vector3::transform(Vector3::UNIT_X, rotation_);
	}
	void rotateToNewForward(const Vector3& forward);
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
	Matrix4 worldTransform_;
	Vector3 position_;
	Vector3 oldPosition_;
	Vector3 velocity_;
	Vector3 oldVelocity_;
	Vector3 acceleration_;
	Quaternion rotation_;
	float scale_;
	bool recomputeTransform_;
	bool moving_;
	std::vector<Component*> components_;
	class Game* game_;
};