//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "Functions.h"

float Math::abs(float value)
{
	return (float)fabs(value);
}
float Math::cos(float angle)
{
	return cosf(angle);
}
float Math::sin(float angle)
{
	return sinf(angle);
}
float Math::tan(float angle)
{
	return tanf(angle);
}
float Math::acos(float value)
{
	return acosf(value);
}
float Math::atan2(float y, float x)
{
	return atan2f(y, x);
}
float Math::cot(float angle)
{
	return 1.0f / tan(angle);
}
float Math::lerp(float a, float b, float f)
{
	return a + f * (b - a);
}
float Math::sqrt(float value)
{
	return sqrtf(value);
}
float Math::mod(float numer, float denom)
{
	return (float)fmod(numer, denom);
}
float Math::toRadians(float degrees)
{
	return degrees * PI / 180.0f;
}
float Math::toDegrees(float radians)
{
	return radians * 180.0f / PI;
}
bool Math::nearZero(float val, float epsilon)
{
	if (fabs(val) <= epsilon)
	{
		return true;
	}
	else
	{
		return false;
	}
}