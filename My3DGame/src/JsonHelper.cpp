//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "JsonHelper.h"

bool JsonHelper::getInt(const rapidjson::Value& inObject, const char* inProperty, int& outInt)
{
	//Check if this property exists
	auto itr = inObject.FindMember(inProperty);
	if(itr == inObject.MemberEnd())
	{
		return false;
	}
	//Get the value type, and check it's an integer
	auto& property = itr->value;
	if(!property.IsInt())
	{
		return false;
	}
	//We have the property
	outInt = property.GetInt();
	return true;
}
bool JsonHelper::getFloat(const rapidjson::Value& inObject, const char* inProperty, float& outFloat)
{
	auto itr = inObject.FindMember(inProperty);
	if(itr == inObject.MemberEnd())
	{
		return false;
	}
	auto& property = itr->value;
	if(!property.IsDouble())
	{
		return false;
	}
	outFloat = property.GetDouble();
	return true;
}
bool JsonHelper::getString(const rapidjson::Value& inObject, const char* inProperty, std::string& outStr)
{
	auto itr = inObject.FindMember(inProperty);
	if(itr == inObject.MemberEnd())
	{
		return false;
	}
	auto& property = itr->value;
	if(!property.IsString())
	{
		return false;
	}
	outStr = property.GetString();
	return true;
}
bool JsonHelper::getBool(const rapidjson::Value& inObject, const char* inProperty, bool& outBool)
{
	auto itr = inObject.FindMember(inProperty);
	if(itr == inObject.MemberEnd())
	{
		return false;
	}
	auto& property = itr->value;
	if(!property.IsBool())
	{
		return false;
	}
	outBool = property.GetBool();
	return true;
}
bool JsonHelper::getVector3(const rapidjson::Value& inObject, const char* inProperty, Vector3& outVector)
{
	auto itr = inObject.FindMember(inProperty);
	if(itr == inObject.MemberEnd())
	{
		return false;
	}
	auto& property = itr->value;
	if(!property.IsArray() || property.Size() != 3)
	{
		return false;
	}
	for(rapidjson::SizeType i = 0; i < 3; i++)
	{
		if(!property[i].IsDouble())
		{
			return false;
		}
	}
	outVector.x = property[0].GetDouble();
	outVector.y = property[1].GetDouble();
	outVector.z = property[2].GetDouble();
	return true;
}
bool JsonHelper::getQuaternion(const rapidjson::Value& inObject, const char* inProperty, Quaternion& outQuat)
{
	auto itr = inObject.FindMember(inProperty);
	if (itr == inObject.MemberEnd())
	{
		return false;
	}
	auto& property = itr->value;
	for(rapidjson::SizeType i = 0; i < 4; i++)
	{
		if(!property[i].IsDouble())
		{
			return false;
		}
	}
	outQuat.x = property[0].GetDouble();
	outQuat.y = property[1].GetDouble();
	outQuat.z = property[2].GetDouble();
	outQuat.w = property[3].GetDouble();
	return true;
}
void JsonHelper::addInt(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObject, const char* name, int value)
{
	rapidjson::Value v(value);
	inObject.AddMember(rapidjson::StringRef(name), v, alloc);
}
void JsonHelper::addFloat(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObject, const char* name, float value)
{
	rapidjson::Value v(value);
	inObject.AddMember(rapidjson::StringRef(name), v, alloc);
}
void JsonHelper::addString(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObject, const char* name, const std::string& value)
{
	rapidjson::Value v;
	v.SetString(value.c_str(), static_cast<rapidjson::SizeType>(value.length()), alloc);
	inObject.AddMember(rapidjson::StringRef(name), v, alloc);
}
void JsonHelper::addBool(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObject, const char* name, bool value)
{
	rapidjson::Value v(value);
	inObject.AddMember(rapidjson::StringRef(name), v, alloc);
}
void JsonHelper::addVector3(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObject, const char* name, const Vector3& value)
{
	//Create an array
	rapidjson::Value v(rapidjson::kArrayType);
	//Push back elements
	v.PushBack(rapidjson::Value(value.x).Move(), alloc);
	v.PushBack(rapidjson::Value(value.y).Move(), alloc);
	v.PushBack(rapidjson::Value(value.z).Move(), alloc);
	//Add array to inObject
	inObject.AddMember(rapidjson::StringRef(name), v, alloc);
}
void JsonHelper::addQuaternion(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObject, const char* name, const Quaternion& value)
{
	//Create an array
	rapidjson::Value v(rapidjson::kArrayType);
	//Push back elements
	v.PushBack(rapidjson::Value(value.x).Move(), alloc);
	v.PushBack(rapidjson::Value(value.y).Move(), alloc);
	v.PushBack(rapidjson::Value(value.z).Move(), alloc);
	v.PushBack(rapidjson::Value(value.w).Move(), alloc);
	//Add array to inObject
	inObject.AddMember(rapidjson::StringRef(name), v, alloc);
}