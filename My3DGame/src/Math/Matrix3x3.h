//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include "Vector2.h"

class Matrix3x3
{
public:
	//Identity Matrix
	float matrix[3][3] =
	{
		{1.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 1.0f}
	};
	Matrix3x3();
	explicit Matrix3x3(float inputMatrix[3][3]);
	//Cast to a const float pointer
	const float* getAsFloatPtr() const;
	//Matrix multiplication
	friend Matrix3x3 operator*(const Matrix3x3& left, const Matrix3x3& right);
	Matrix3x3& operator*=(const Matrix3x3& right);
};
namespace Math
{
	//Create a scale matrix with x and y scales
	Matrix3x3 createScaleMatrix(float xScale, float yScale);
	//Create a scale matrix (on the XY-plane)
	Matrix3x3 createScaleMatrix(const Vector2& vec);
	//Create a rotation matrix about the Z axis (theta is in radians)
	Matrix3x3 createRotationMatrix(float theta);
	//Create a translation matrix with x and y
	Matrix3x3 createTranslationMatrix(float xTrans, float yTrans);
	//Create a translation matrix (on the XY-plane)
	Matrix3x3 createTranslationMatrix(const Vector2& vec);
};