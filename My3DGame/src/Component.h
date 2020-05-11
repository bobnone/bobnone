//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include <rapidjson/document.h>

class Component
{
public:
	enum TypeID
	{
		TComponent = 0,
		TAudioComponent,
		TBoxComponent,
		TCameraComponent,
		TFollowCamera,
		TMeshComponent,
		TMoveComponent,
		TSkeletalMeshComponent,
		TSpriteComponent,
		TMirrorCamera,
		TPointLightComponent,
		TTargetComponent,
		NUM_COMPONENT_TYPES
	};
	static const char* TypeNames[NUM_COMPONENT_TYPES];
	/*Constructor
	Note: the lower the update order, the earlier the component updates*/
	Component(class Actor* owner, int updateOrder = 100);
	//Destructor
	virtual ~Component();
	//Update this component by delta time
	virtual void update(float deltaTime);
	//Process input for this component
	virtual void processInput(const uint8_t* keyState)
	{
		//EMPTY:
	}
	//Called when world transform changes
	virtual void onUpdateWorldTransform();
	class Actor* owner()
	{
		return owner_;
	}
	int updateOrder() const
	{
		return updateOrder_;
	}
	virtual TypeID getType() const = 0;
	//Load/Save
	virtual void loadProperties(const rapidjson::Value& inObj);
	virtual void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const;
	//Create a component with specified properties
	template <typename T>
	static Component* create(class Actor* actor, const rapidjson::Value& inObj)
	{
		//Dynamically allocate component of type T
		T* t = new T(actor);
		//Call LoadProperties on new component
		t->loadProperties(inObj);
		return t;
	}
protected:
	//Owning actor
	class Actor* owner_;
	//Update order of component
	int updateOrder_;
};