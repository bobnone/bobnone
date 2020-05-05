#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include "JsonHelper.h"

const char* Actor::TypeNames[NUM_ACTOR_TYPES] = {
	"Actor",
	"FollowActor",
	"PlaneActor",
	"TargetActor",
};

Actor::Actor(Game* game) :mState(ACTOR_ACTIVE), mPosition(vector3::Zero), mOldPosition(vector3::Zero), mVelocity(vector3::Zero), mOldVelocity(vector3::Zero), mAcceleration(vector3::Zero), mRotation(quaternion()), mScale(1.0f), mGame(game), mRecomputeTransform(true), mMoving(false)
{
	mGame->AddActor(this);
}
Actor::~Actor()
{
	mGame->RemoveActor(this);
	// Need to delete components
	// Because ~Component calls RemoveComponent, need a different style loop
	while (!mComponents.empty())
	{
		delete mComponents.back();
	}
}
void Actor::Update(float deltaTime)
{
	if (mState == ACTOR_ACTIVE)
	{
		if (mRecomputeTransform)
		{
			// Calculate the change in velocity and Acceleration
			if (mOldPosition != mPosition)
			{
				// Calculate velocity
				// Note: velocity = (change in position)/time
				vector3 positionChange(mOldPosition.x - mPosition.x, mOldPosition.y - mPosition.y, mOldPosition.z - mPosition.z);
				mVelocity.x = positionChange.x / deltaTime;
				mVelocity.y = positionChange.y / deltaTime;
				mVelocity.z = positionChange.z / deltaTime;
				mOldPosition = mPosition;
				// Calculate acceleration
				// Note: acceleration = (change in velocity)/time
				vector3 velocityChange(mOldVelocity.x - mVelocity.x, mOldVelocity.y - mVelocity.y, mOldVelocity.z - mVelocity.z);
				mAcceleration.x = velocityChange.x / deltaTime;
				mAcceleration.y = velocityChange.y / deltaTime;
				mAcceleration.z = velocityChange.z / deltaTime;
				mOldVelocity = mVelocity;
				mMoving = true;
			}
			else
			{
				mMoving = false;
			}
			ComputeWorldTransform();
		}
		UpdateComponents(deltaTime);
		UpdateActor(deltaTime);
	}
}
void Actor::UpdateComponents(float deltaTime)
{
	for (auto comp : mComponents)
	{
		comp->Update(deltaTime);
	}
}
void Actor::UpdateActor(float deltaTime)
{
	//
}
void Actor::ProcessInput(const uint8_t* keyState)
{
	if (mState == ACTOR_ACTIVE)
	{
		// First process input for components
		for (auto comp : mComponents)
		{
			comp->ProcessInput(keyState);
		}
		ActorInput(keyState);
	}
}
void Actor::ActorInput(const uint8_t* keyState)
{
	//
}
void Actor::ComputeWorldTransform()
{
	mRecomputeTransform = false;
	// Scale, then rotate, then translate
	mWorldTransform = matrix4::CreateScale(mScale);
	mWorldTransform *= matrix4::CreateFromQuaternion(mRotation);
	mWorldTransform *= matrix4::CreateTranslation(mPosition);
	// Inform components world transform updated
	for (auto comp : mComponents)
	{
		comp->OnUpdateWorldTransform();
	}
}
void Actor::RotateToNewForward(const vector3& forward)
{
	// Figure out difference between original (unit x) and new
	float dot = vector3::Dot(vector3::UnitX, forward);
	float angle = Math::Acos(dot);
	// Facing down X
	if (dot > 0.9999f)
	{
		SetRotation(quaternion());
	}
	// Facing down -X
	else if (dot < -0.9999f)
	{
		SetRotation(quaternion(vector3::UnitZ, Math::Pi));
	}
	else
	{
		// Rotate about axis from cross product
		vector3 axis = vector3::Cross(vector3::UnitX, forward);
		axis.Normalize();
		SetRotation(quaternion(axis, angle));
	}
}
void Actor::AddComponent(Component* component)
{
	// Find the insertion point in the sorted vector
	// (The first element with a order higher than me)
	int myOrder = component->GetUpdateOrder();
	auto i = mComponents.begin();
	for (; i != mComponents.end(); ++i)
	{
		if (myOrder < (*i)->GetUpdateOrder())
		{
			break;
		}
	}

	// Inserts element before position of iterator
	mComponents.insert(i, component);
}
void Actor::RemoveComponent(Component* component)
{
	auto i = std::find(mComponents.begin(), mComponents.end(), component);
	if (i != mComponents.end())
	{
		mComponents.erase(i);
	}
}
void Actor::LoadProperties(const rapidjson::Value& inObj)
{
	// Use strings for different states
	std::string state;
	if (JsonHelper::GetString(inObj, "state", state))
	{
		if (state == "active")
		{
			SetState(ACTOR_ACTIVE);
		}
		else if (state == "paused")
		{
			SetState(ACTOR_PAUSED);
		}
		else if (state == "dead")
		{
			SetState(ACTOR_DEAD);
		}
	}
	// Load position, rotation, and scale, and compute transform
	JsonHelper::GetVector3(inObj, "position", mPosition);
	JsonHelper::GetVector3(inObj, "velocity", mVelocity);
	JsonHelper::GetVector3(inObj, "acceleration", mAcceleration);
	JsonHelper::GetQuaternion(inObj, "rotation", mRotation);
	JsonHelper::GetFloat(inObj, "scale", mScale);
	JsonHelper::GetBool(inObj, "moving", mMoving);
	// Set the Old/last position and velocity to the current position and velocity to avoid strange position jumps when loading
	mOldPosition = mPosition;
	mOldVelocity = mVelocity;	
	ComputeWorldTransform();
}
void Actor::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const
{
	std::string state = "active";
	if (mState == ACTOR_PAUSED)
	{
		state = "paused";
	}
	else if (mState == ACTOR_DEAD)
	{
		state = "dead";
	}
	JsonHelper::AddString(alloc, inObj, "state", state);
	JsonHelper::AddVector3(alloc, inObj, "position", mPosition);
	JsonHelper::AddVector3(alloc, inObj, "velocity", mVelocity);
	JsonHelper::AddVector3(alloc, inObj, "acceleration", mAcceleration);
	JsonHelper::AddQuaternion(alloc, inObj, "rotation", mRotation);
	JsonHelper::AddFloat(alloc, inObj, "scale", mScale);
	JsonHelper::AddBool(alloc, inObj, "moving", mMoving);
}