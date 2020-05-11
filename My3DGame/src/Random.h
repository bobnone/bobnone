//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma  once
#include <random>
#include "Math.h"

class Random
{
public:
	static void init();
	//Seed the generator with the specified int
	//NOTE: You should generally not need to manually use this
	static void seed(unsigned int seed);
	//Get a float between 0.0f and 1.0f
	static float getFloat();
	//Get a float from the specified range
	static float getFloatRange(float min, float max);
	//Get an int from the specified range
	static int getIntRange(int min, int max);
	//Get a random vector given the min/max bounds
	static vector2 getVector(const vector2& min, const vector2& max);
	static vector3 getVector(const vector3& min, const vector3& max);
private:
	static std::mt19937 sGenerator;
};
