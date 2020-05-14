//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include <limits>

namespace Math
{
	const float EPSILON = std::numeric_limits<float>::epsilon();
	const float PI = 3.1415926535f;
	const float TWO_PI = PI * 2.0f;
	const float PI_OVER_TWO = PI / 2.0f;
	const float INFINITE_FLOAT = std::numeric_limits<float>::infinity();
	const float NEGATIVE_INFINITE_FLOAT = -std::numeric_limits<float>::infinity();
};