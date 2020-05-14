//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include "Constants.h"

namespace Math
{
	template <typename T>
	T maximum(const T& a, const T& b)
	{
		return (a < b ? b : a);
	}
	template <typename T>
	T minimum(const T& a, const T& b)
	{
		return (a < b ? a : b);
	}
	template <typename T>
	T clamp(const T& value, const T& lower, const T& upper)
	{
		return minimum(upper, maximum(lower, value));
	}
	float abs(float value);
	float cos(float angle);
	float sin(float angle);
	float tan(float angle);
	float acos(float value);
	float atan2(float y, float x);
	float cot(float angle);
	float lerp(float a, float b, float f);
	float sqrt(float value);
	float mod(float numer, float denom);
	float toRadians(float degrees);
	float toDegrees(float radians);
	bool nearZero(float val, float epsilon = 0.001f);
};