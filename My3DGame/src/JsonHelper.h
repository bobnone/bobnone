//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include <rapidjson/document.h>
#include "Math/Math.h"

class JsonHelper
{
public:
	//Helpers - Return true if successful, and also sets out parameter to parsed value
	//For each function, the first parameter is the containing JSON object, the second is the name of the property in the containing object, and the third is the value you acquire.
	//Furthermore, if the property is not found, each function is guaranteed not to modify the return value.
	static bool getInt(const rapidjson::Value& inObject, const char* inProperty, int& outInt);
	static bool getFloat(const rapidjson::Value& inObject, const char* inProperty, float& outFloat);
	static bool getString(const rapidjson::Value& inObject, const char* inProperty, std::string& outStr);
	static bool getBool(const rapidjson::Value& inObject, const char* inProperty, bool& outBool);
	static bool getVector3(const rapidjson::Value& inObject, const char* inProperty, Vector3& outVector);
	static bool getQuaternion(const rapidjson::Value& inObject, const char* inProperty, Quaternion& outQuat);
	//Setter functions
	static void addInt(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObject, const char* name, int value);
	static void addFloat(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObject, const char* name, float value);
	static void addString(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObject, const char* name, const std::string& value);
	static void addBool(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObject, const char* name, bool value);
	static void addVector3(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObject, const char* name, const Vector3& value);
	static void addQuaternion(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObject, const char* name, const Quaternion& value);
};