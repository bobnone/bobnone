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

Actor::Actor(Game* game):state_(ACTOR_ACTIVE), position_(Vector3()), oldPosition_(Vector3()), velocity_(Vector3()), oldVelocity_(Vector3()), acceleration_(Vector3()), rotation_(Quaternion()), scale_(1.0f), game_(game), recomputeTransform_(true), moving_(false)
{
	game_->addActor(this);
}
Actor::~Actor()
{
	game_->removeActor(this);
	//Need to delete components
	//Because ~Component calls RemoveComponent, need a different style loop
	while(!components_.empty())
	{
		delete components_.back();
	}
}
void Actor::update(float deltaTime)
{
	if(state_ == ACTOR_ACTIVE)
	{
		if(recomputeTransform_)
		{
			//Calculate the change in velocity and Acceleration
			if(oldPosition_ != position_)
			{
				//Calculate velocity
				//Note: velocity = (change in position)/time
				Vector3 positionChange(oldPosition_.x - position_.x, oldPosition_.y - position_.y, oldPosition_.z - position_.z);
				velocity_.x = positionChange.x / deltaTime;
				velocity_.y = positionChange.y / deltaTime;
				velocity_.z = positionChange.z / deltaTime;
				oldPosition_ = position_;
				//Calculate acceleration
				//Note: acceleration = (change in velocity)/time
				Vector3 velocityChange(oldVelocity_.x - velocity_.x, oldVelocity_.y - velocity_.y, oldVelocity_.z - velocity_.z);
				acceleration_.x = velocityChange.x / deltaTime;
				acceleration_.y = velocityChange.y / deltaTime;
				acceleration_.z = velocityChange.z / deltaTime;
				oldVelocity_ = velocity_;
				moving_ = true;
			}
			else
			{
				moving_ = false;
			}
			computeWorldTransform();
		}
		updateComponents(deltaTime);
		updateActor(deltaTime);
	}
}
void Actor::updateComponents(float deltaTime)
{
	for(auto comp : components_)
	{
		comp->update(deltaTime);
	}
}
void Actor::updateActor(float deltaTime)
{
	//EMPTY:
}
void Actor::processInput(const uint8_t* keyState)
{
	if(state_ == ACTOR_ACTIVE)
	{
		// First process input for components
		for(auto comp : components_)
		{
			comp->processInput(keyState);
		}
		actorInput(keyState);
	}
}
void Actor::actorInput(const uint8_t* keyState)
{
	//EMPTY:
}
void Actor::computeWorldTransform()
{
	recomputeTransform_ = false;
	//Scale, then rotate, then translate
	worldTransform_ = Matrix4::createScale(scale_);
	worldTransform_ *= Matrix4::fromQuaternion(rotation_);
	worldTransform_ *= Matrix4::createTranslation(position_);
	//Inform components world transform updated
	for(auto comp : components_)
	{
		comp->onUpdateWorldTransform();
	}
}
void Actor::rotateToNewForward(const Vector3& forward)
{
	//Figure out difference between original (unit x) and new
	float dot = Vector3::dot(Vector3::UNIT_X, forward);
	float angle = Math::acos(dot);
	//Facing down X
	if(dot > 0.9999f)
	{
		setRotation(Quaternion());
	}
	//Facing down -X
	else if (dot < -0.9999f)
	{
		setRotation(Quaternion(Vector3::UNIT_Z, Math::PI));
	}
	else
	{
		//Rotate about axis from cross product
		Vector3 axis = Vector3::cross(Vector3::UNIT_X, forward);
		axis.normalize();
		setRotation(Quaternion(axis, angle));
	}
}
void Actor::addComponent(Component* component)
{
	//Find the insertion point in the sorted vector (The first element with a order higher than me)
	int myOrder = component->updateOrder();
	auto i = components_.begin();
	for(; i != components_.end(); ++i)
	{
		if(myOrder < (*i)->updateOrder())
		{
			break;
		}
	}
	//Inserts element before position of iterator
	components_.insert(i, component);
}
void Actor::removeComponent(Component* component)
{
	auto i = std::find(components_.begin(), components_.end(), component);
	if(i != components_.end())
	{
		components_.erase(i);
	}
}
void Actor::loadProperties(const rapidjson::Value& inObj)
{
	//Use strings for different states
	std::string state;
	if(JsonHelper::getString(inObj, "state", state))
	{
		if (state == "active")
		{
			setState(ACTOR_ACTIVE);
		}
		else if(state == "paused")
		{
			setState(ACTOR_PAUSED);
		}
		else if(state == "dead")
		{
			setState(ACTOR_DEAD);
		}
	}
	//Load position, rotation, and scale, and compute transform
	JsonHelper::getVector3(inObj, "position", position_);
	JsonHelper::getVector3(inObj, "velocity", velocity_);
	JsonHelper::getVector3(inObj, "acceleration", acceleration_);
	JsonHelper::getQuaternion(inObj, "rotation", rotation_);
	JsonHelper::getFloat(inObj, "scale", scale_);
	JsonHelper::getBool(inObj, "moving", moving_);
	//Set the Old/last position and velocity to the current position and velocity to avoid strange position jumps when loading
	oldPosition_ = position_;
	oldVelocity_ = velocity_;
	computeWorldTransform();
}
void Actor::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const
{
	std::string state = "active";
	if (state_ == ACTOR_PAUSED)
	{
		state = "paused";
	}
	else if (state_ == ACTOR_DEAD)
	{
		state = "dead";
	}
	JsonHelper::addString(alloc, inObj, "state", state);
	JsonHelper::addVector3(alloc, inObj, "position", position_);
	JsonHelper::addVector3(alloc, inObj, "velocity", velocity_);
	JsonHelper::addVector3(alloc, inObj, "acceleration", acceleration_);
	JsonHelper::addQuaternion(alloc, inObj, "rotation", rotation_);
	JsonHelper::addFloat(alloc, inObj, "scale", scale_);
	JsonHelper::addBool(alloc, inObj, "moving", moving_);
}