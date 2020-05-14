//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include "Vector3.h"

class Matrix4x4
{
public:
	//Identity Matrix
	float matrix[4][4] =
	{
		{ 1.0f, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f }
	};
	Matrix4x4();
	explicit Matrix4x4(float inputMatrix[4][4]);
	//Cast to a const float pointer
	const float* getAsFloatPtr() const;
	//Matrix multiplication (a * b)
	friend Matrix4x4 operator*(const Matrix4x4& a, const Matrix4x4& b);
	Matrix4x4& operator*=(const Matrix4x4& right);
	//Invert the matrix - super slow
	void invert();
	//Get the translation component of the matrix
	Vector3 getTranslation() const;
	//Get the X axis of the matrix (right)
	Vector3 getXAxis() const;
	//Get the Y axis of the matrix (up)
	Vector3 getYAxis() const;
	//Get the Z axis of the matrix (forward)
	Vector3 getZAxis() const;
	//Extract the scale component from the matrix
	Vector3 getScale() const;
};
namespace Math
{
	//Create a scale matrix with x, y, and z scales
	Matrix4x4 createScaleMatrix(float xScale, float yScale, float zScale);
	//Create a scale matrix (on the XYZ-plane)
	Matrix4x4 createScaleMatrix(const Vector3& vec);
	//Create a scale matrix with a uniform factor
	Matrix4x4 createScaleMatrix(float scale);
	//Rotation about x-axis
	Matrix4x4 createRotationMatrixX(float theta);
	//Rotation about y-axis
	Matrix4x4 createRotationMatrixY(float theta);
	//Rotation about z-axis
	Matrix4x4 CreateRotationMatrixZ(float theta);
	//Create a translation matrix with x, y, and z
	Matrix4x4 createTranslationMatrix(float xTrans, float yTrans, float zTrans);
	//Create a translation matrix (on the XYZ-plane)
	Matrix4x4 createTranslationMatrix(const Vector3& vec);
	//Create a translation matrix with a uniform factor
	Matrix4x4 createTranslationMatrix(float trans);
	Matrix4x4 lookAt(const Vector3& eye, const Vector3& target, const Vector3& up);
	Matrix4x4 ortho(float width, float height, float near, float far);
	Matrix4x4 perspectiveFOV(float fovY, float width, float height, float near, float far);
	//Create "Simple" View-Projection Matrix from Chapter 6
	Matrix4x4 createSimpleViewProjectionMatrix(float width, float height);
};