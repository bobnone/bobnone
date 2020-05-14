//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "Matrix3x3.h"

Matrix3x3::Matrix3x3()
{
	//EMPTY:
}
Matrix3x3::Matrix3x3(float inputMatrix[3][3])
{
	memcpy(matrix, inputMatrix, 9 * sizeof(float));
}
const float* Matrix3x3::getAsFloatPtr() const
{
	return reinterpret_cast<const float*>(&matrix[0][0]);
}
Matrix3x3 operator*(const Matrix3x3& left, const Matrix3x3& right)
{
	Matrix3x3 retVal;
	//row 0
	retVal.matrix[0][0] = left.matrix[0][0] * right.matrix[0][0] + left.matrix[0][1] * right.matrix[1][0] + left.matrix[0][2] * right.matrix[2][0];
	retVal.matrix[0][1] = left.matrix[0][0] * right.matrix[0][1] + left.matrix[0][1] * right.matrix[1][1] + left.matrix[0][2] * right.matrix[2][1];
	retVal.matrix[0][2] = left.matrix[0][0] * right.matrix[0][2] + left.matrix[0][1] * right.matrix[1][2] + left.matrix[0][2] * right.matrix[2][2];
	//row 1
	retVal.matrix[1][0] = left.matrix[1][0] * right.matrix[0][0] + left.matrix[1][1] * right.matrix[1][0] + left.matrix[1][2] * right.matrix[2][0];
	retVal.matrix[1][1] = left.matrix[1][0] * right.matrix[0][1] + left.matrix[1][1] * right.matrix[1][1] + left.matrix[1][2] * right.matrix[2][1];
	retVal.matrix[1][2] = left.matrix[1][0] * right.matrix[0][2] + left.matrix[1][1] * right.matrix[1][2] + left.matrix[1][2] * right.matrix[2][2];
	//row 2
	retVal.matrix[2][0] = left.matrix[2][0] * right.matrix[0][0] + left.matrix[2][1] * right.matrix[1][0] + left.matrix[2][2] * right.matrix[2][0];
	retVal.matrix[2][1] = left.matrix[2][0] * right.matrix[0][1] + left.matrix[2][1] * right.matrix[1][1] + left.matrix[2][2] * right.matrix[2][1];
	retVal.matrix[2][2] = left.matrix[2][0] * right.matrix[0][2] + left.matrix[2][1] * right.matrix[1][2] + left.matrix[2][2] * right.matrix[2][2];
	return retVal;
}
Matrix3x3& Matrix3x3::operator*=(const Matrix3x3& right)
{
	*this = *this * right;
	return *this;
}
Matrix3x3 Math::createScaleMatrix(float xScale, float yScale)
{
	float temp[3][3] =
	{
		{ xScale, 0.0f, 0.0f },
		{ 0.0f, yScale, 0.0f },
		{ 0.0f, 0.0f, 1.0f },
	};
	return Matrix3x3(temp);
}
Matrix3x3 Math::createScaleMatrix(const Vector2& vec)
{
	return createScaleMatrix(vec.x, vec.y);
}
Matrix3x3 Math::createRotationMatrix(float theta)
{
	float temp[3][3] =
	{
		{ Math::cos(theta), Math::sin(theta), 0.0f },
		{ -Math::sin(theta), Math::cos(theta), 0.0f },
		{ 0.0f, 0.0f, 1.0f },
	};
	return Matrix3x3(temp);
}
Matrix3x3 Math::createTranslationMatrix(float xTrans, float yTrans)
{
	float temp[3][3] =
	{
		{ 1.0f, 0.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f },
		{ xTrans, yTrans, 1.0f },
	};
	return Matrix3x3(temp);
}
Matrix3x3 Math::createTranslationMatrix(const Vector2& vec)
{
	return createTranslationMatrix(vec.x, vec.y);
}