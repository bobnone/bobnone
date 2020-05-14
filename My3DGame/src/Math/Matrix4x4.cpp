//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include "Matrix4x4.h"

Matrix4x4::Matrix4x4()
{
	//EMPTY:
}
Matrix4x4::Matrix4x4(float inputMatrix[4][4])
{
	memcpy(matrix, inputMatrix, 16 * sizeof(float));
}
const float* Matrix4x4::getAsFloatPtr() const
{
	return reinterpret_cast<const float*>(&matrix[0][0]);
}
Matrix4x4 operator*(const Matrix4x4& a, const Matrix4x4& b)
{
	Matrix4x4 retVal;
	// row 0
	retVal.matrix[0][0] = a.matrix[0][0] * b.matrix[0][0] + a.matrix[0][1] * b.matrix[1][0] + a.matrix[0][2] * b.matrix[2][0] + a.matrix[0][3] * b.matrix[3][0];
	retVal.matrix[0][1] = a.matrix[0][0] * b.matrix[0][1] + a.matrix[0][1] * b.matrix[1][1] + a.matrix[0][2] * b.matrix[2][1] + a.matrix[0][3] * b.matrix[3][1];
	retVal.matrix[0][2] = a.matrix[0][0] * b.matrix[0][2] + a.matrix[0][1] * b.matrix[1][2] + a.matrix[0][2] * b.matrix[2][2] + a.matrix[0][3] * b.matrix[3][2];
	retVal.matrix[0][3] = a.matrix[0][0] * b.matrix[0][3] + a.matrix[0][1] * b.matrix[1][3] + a.matrix[0][2] * b.matrix[2][3] + a.matrix[0][3] * b.matrix[3][3];
	// row 1
	retVal.matrix[1][0] = a.matrix[1][0] * b.matrix[0][0] + a.matrix[1][1] * b.matrix[1][0] + a.matrix[1][2] * b.matrix[2][0] + a.matrix[1][3] * b.matrix[3][0];
	retVal.matrix[1][1] = a.matrix[1][0] * b.matrix[0][1] + a.matrix[1][1] * b.matrix[1][1] + a.matrix[1][2] * b.matrix[2][1] + a.matrix[1][3] * b.matrix[3][1];
	retVal.matrix[1][2] = a.matrix[1][0] * b.matrix[0][2] + a.matrix[1][1] * b.matrix[1][2] + a.matrix[1][2] * b.matrix[2][2] + a.matrix[1][3] * b.matrix[3][2];
	retVal.matrix[1][3] = a.matrix[1][0] * b.matrix[0][3] + a.matrix[1][1] * b.matrix[1][3] + a.matrix[1][2] * b.matrix[2][3] + a.matrix[1][3] * b.matrix[3][3];
	// row 2
	retVal.matrix[2][0] = a.matrix[2][0] * b.matrix[0][0] + a.matrix[2][1] * b.matrix[1][0] + a.matrix[2][2] * b.matrix[2][0] + a.matrix[2][3] * b.matrix[3][0];
	retVal.matrix[2][1] = a.matrix[2][0] * b.matrix[0][1] + a.matrix[2][1] * b.matrix[1][1] + a.matrix[2][2] * b.matrix[2][1] + a.matrix[2][3] * b.matrix[3][1];
	retVal.matrix[2][2] = a.matrix[2][0] * b.matrix[0][2] + a.matrix[2][1] * b.matrix[1][2] + a.matrix[2][2] * b.matrix[2][2] + a.matrix[2][3] * b.matrix[3][2];
	retVal.matrix[2][3] = a.matrix[2][0] * b.matrix[0][3] + a.matrix[2][1] * b.matrix[1][3] + a.matrix[2][2] * b.matrix[2][3] + a.matrix[2][3] * b.matrix[3][3];
	// row 3
	retVal.matrix[3][0] = a.matrix[3][0] * b.matrix[0][0] + a.matrix[3][1] * b.matrix[1][0] + a.matrix[3][2] * b.matrix[2][0] + a.matrix[3][3] * b.matrix[3][0];
	retVal.matrix[3][1] = a.matrix[3][0] * b.matrix[0][1] + a.matrix[3][1] * b.matrix[1][1] + a.matrix[3][2] * b.matrix[2][1] + a.matrix[3][3] * b.matrix[3][1];
	retVal.matrix[3][2] = a.matrix[3][0] * b.matrix[0][2] + a.matrix[3][1] * b.matrix[1][2] + a.matrix[3][2] * b.matrix[2][2] + a.matrix[3][3] * b.matrix[3][2];
	retVal.matrix[3][3] = a.matrix[3][0] * b.matrix[0][3] + a.matrix[3][1] * b.matrix[1][3] + a.matrix[3][2] * b.matrix[2][3] + a.matrix[3][3] * b.matrix[3][3];
	return retVal;
}
Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& right)
{
	*this = *this * right;
	return *this;
}
void Matrix4x4::invert()
{
	//This is a really slow and unstable way to unroll everything...
	float tmp[12];
	float src[16];
	float dst[16];
	float det;
	//Transpose matrix:
	//row 1 to col 1
	src[0] = matrix[0][0];
	src[4] = matrix[0][1];
	src[8] = matrix[0][2];
	src[12] = matrix[0][3];
	//row 2 to col 2
	src[1] = matrix[1][0];
	src[5] = matrix[1][1];
	src[9] = matrix[1][2];
	src[13] = matrix[1][3];
	//row 3 to col 3
	src[2] = matrix[2][0];
	src[6] = matrix[2][1];
	src[10] = matrix[2][2];
	src[14] = matrix[2][3];
	//row 4 to col 4
	src[3] = matrix[3][0];
	src[7] = matrix[3][1];
	src[11] = matrix[3][2];
	src[15] = matrix[3][3];
	//Calculate cofactors:
	tmp[0] = src[10] * src[15];
	tmp[1] = src[11] * src[14];
	tmp[2] = src[9] * src[15];
	tmp[3] = src[11] * src[13];
	tmp[4] = src[9] * src[14];
	tmp[5] = src[10] * src[13];
	tmp[6] = src[8] * src[15];
	tmp[7] = src[11] * src[12];
	tmp[8] = src[8] * src[14];
	tmp[9] = src[10] * src[12];
	tmp[10] = src[8] * src[13];
	tmp[11] = src[9] * src[12];
	dst[0] = tmp[0] * src[5] + tmp[3] * src[6] + tmp[4] * src[7];
	dst[0] -= tmp[1] * src[5] + tmp[2] * src[6] + tmp[5] * src[7];
	dst[1] = tmp[1] * src[4] + tmp[6] * src[6] + tmp[9] * src[7];
	dst[1] -= tmp[0] * src[4] + tmp[7] * src[6] + tmp[8] * src[7];
	dst[2] = tmp[2] * src[4] + tmp[7] * src[5] + tmp[10] * src[7];
	dst[2] -= tmp[3] * src[4] + tmp[6] * src[5] + tmp[11] * src[7];
	dst[3] = tmp[5] * src[4] + tmp[8] * src[5] + tmp[11] * src[6];
	dst[3] -= tmp[4] * src[4] + tmp[9] * src[5] + tmp[10] * src[6];
	dst[4] = tmp[1] * src[1] + tmp[2] * src[2] + tmp[5] * src[3];
	dst[4] -= tmp[0] * src[1] + tmp[3] * src[2] + tmp[4] * src[3];
	dst[5] = tmp[0] * src[0] + tmp[7] * src[2] + tmp[8] * src[3];
	dst[5] -= tmp[1] * src[0] + tmp[6] * src[2] + tmp[9] * src[3];
	dst[6] = tmp[3] * src[0] + tmp[6] * src[1] + tmp[11] * src[3];
	dst[6] -= tmp[2] * src[0] + tmp[7] * src[1] + tmp[10] * src[3];
	dst[7] = tmp[4] * src[0] + tmp[9] * src[1] + tmp[10] * src[2];
	dst[7] -= tmp[5] * src[0] + tmp[8] * src[1] + tmp[11] * src[2];
	tmp[0] = src[2] * src[7];
	tmp[1] = src[3] * src[6];
	tmp[2] = src[1] * src[7];
	tmp[3] = src[3] * src[5];
	tmp[4] = src[1] * src[6];
	tmp[5] = src[2] * src[5];
	tmp[6] = src[0] * src[7];
	tmp[7] = src[3] * src[4];
	tmp[8] = src[0] * src[6];
	tmp[9] = src[2] * src[4];
	tmp[10] = src[0] * src[5];
	tmp[11] = src[1] * src[4];
	dst[8] = tmp[0] * src[13] + tmp[3] * src[14] + tmp[4] * src[15];
	dst[8] -= tmp[1] * src[13] + tmp[2] * src[14] + tmp[5] * src[15];
	dst[9] = tmp[1] * src[12] + tmp[6] * src[14] + tmp[9] * src[15];
	dst[9] -= tmp[0] * src[12] + tmp[7] * src[14] + tmp[8] * src[15];
	dst[10] = tmp[2] * src[12] + tmp[7] * src[13] + tmp[10] * src[15];
	dst[10] -= tmp[3] * src[12] + tmp[6] * src[13] + tmp[11] * src[15];
	dst[11] = tmp[5] * src[12] + tmp[8] * src[13] + tmp[11] * src[14];
	dst[11] -= tmp[4] * src[12] + tmp[9] * src[13] + tmp[10] * src[14];
	dst[12] = tmp[2] * src[10] + tmp[5] * src[11] + tmp[1] * src[9];
	dst[12] -= tmp[4] * src[11] + tmp[0] * src[9] + tmp[3] * src[10];
	dst[13] = tmp[8] * src[11] + tmp[0] * src[8] + tmp[7] * src[10];
	dst[13] -= tmp[6] * src[10] + tmp[9] * src[11] + tmp[1] * src[8];
	dst[14] = tmp[6] * src[9] + tmp[11] * src[11] + tmp[3] * src[8];
	dst[14] -= tmp[10] * src[11] + tmp[2] * src[8] + tmp[7] * src[9];
	dst[15] = tmp[10] * src[10] + tmp[4] * src[8] + tmp[9] * src[9];
	dst[15] -= tmp[8] * src[9] + tmp[11] * src[10] + tmp[5] * src[8];
	//Calculate determinant
	det = src[0] * dst[0] + src[1] * dst[1] + src[2] * dst[2] + src[3] * dst[3];
	//Inverse of matrix is divided by determinant
	det = 1 / det;
	for(int j = 0; j < 16; j++)
	{
		dst[j] *= det;
	}
	//Set it back
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			matrix[i][j] = dst[i * 4 + j];
		}
	}
}
Vector3 Matrix4x4::getTranslation() const
{
	return Vector3(matrix[3][0], matrix[3][1], matrix[3][2]);
}
Vector3 Matrix4x4::getXAxis() const
{
	return Math::normalize(Vector3(matrix[0][0], matrix[0][1], matrix[0][2]));
}
Vector3 Matrix4x4::getYAxis() const
{
	return Math::normalize(Vector3(matrix[1][0], matrix[1][1], matrix[1][2]));
}
Vector3 Matrix4x4::getZAxis() const
{
	return Math::normalize(Vector3(matrix[2][0], matrix[2][1], matrix[2][2]));
}
Vector3 Matrix4x4::getScale() const
{
	Vector3 retVal;
	retVal.x = Vector3(matrix[0][0], matrix[0][1], matrix[0][2]).length();
	retVal.y = Vector3(matrix[1][0], matrix[1][1], matrix[1][2]).length();
	retVal.z = Vector3(matrix[2][0], matrix[2][1], matrix[2][2]).length();
	return retVal;
}
Matrix4x4 Math::createScaleMatrix(float xScale, float yScale, float zScale)
{
	float temp[4][4] =
	{
		{ xScale, 0.0f, 0.0f, 0.0f },
		{ 0.0f, yScale, 0.0f, 0.0f },
		{ 0.0f, 0.0f, zScale, 0.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f }
	};
	return Matrix4x4(temp);
}
Matrix4x4 Math::createScaleMatrix(const Vector3& scaleVector)
{
	return createScaleMatrix(scaleVector.x, scaleVector.y, scaleVector.z);
}
Matrix4x4 Math::createScaleMatrix(float scale)
{
	return createScaleMatrix(scale, scale, scale);
}
Matrix4x4 Math::createRotationMatrixX(float theta)
{
	float temp[4][4] =
	{
		{ 1.0f, 0.0f, 0.0f , 0.0f },
		{ 0.0f, Math::cos(theta), Math::sin(theta), 0.0f },
		{ 0.0f, -Math::sin(theta), Math::cos(theta), 0.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f },
	};
	return Matrix4x4(temp);
}
Matrix4x4 Math::createRotationMatrixY(float theta)
{
	float temp[4][4] =
	{
		{ Math::cos(theta), 0.0f, -Math::sin(theta), 0.0f },
		{ 0.0f, 1.0f, 0.0f, 0.0f },
		{ Math::sin(theta), 0.0f, Math::cos(theta), 0.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f },
	};
	return Matrix4x4(temp);
}
Matrix4x4 Math::CreateRotationMatrixZ(float theta)
{
	float temp[4][4] =
	{
		{ Math::cos(theta), Math::sin(theta), 0.0f, 0.0f },
		{ -Math::sin(theta), Math::cos(theta), 0.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f },
	};
	return Matrix4x4(temp);
}
Matrix4x4 Math::createTranslationMatrix(float xTrans, float yTrans, float zTrans)
{
	float temp[4][4] =
	{
		{ 1.0f, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f, 0.0f },
		{ xTrans, yTrans, zTrans, 1.0f }
	};
	return Matrix4x4(temp);
}
Matrix4x4 Math::createTranslationMatrix(const Vector3& vec)
{
	return createTranslationMatrix(vec.x, vec.y, vec.z);
}
Matrix4x4 Math::createTranslationMatrix(float trans)
{
	return createTranslationMatrix(trans, trans, trans);
}
Matrix4x4 Math::lookAt(const Vector3& eye, const Vector3& target, const Vector3& up)
{
	Vector3 zaxis = Math::normalize(target - eye);
	Vector3 xaxis = Math::normalize(Math::cross(up, zaxis));
	Vector3 yaxis = Math::normalize(Math::cross(zaxis, xaxis));
	Vector3 trans;
	trans.x = -Math::dot(xaxis, eye);
	trans.y = -Math::dot(yaxis, eye);
	trans.z = -Math::dot(zaxis, eye);
	float temp[4][4] =
	{
		{ xaxis.x, yaxis.x, zaxis.x, 0.0f },
		{ xaxis.y, yaxis.y, zaxis.y, 0.0f },
		{ xaxis.z, yaxis.z, zaxis.z, 0.0f },
		{ trans.x, trans.y, trans.z, 1.0f }
	};
	return Matrix4x4(temp);
}
Matrix4x4 Math::ortho(float width, float height, float near, float far)
{
	float temp[4][4] =
	{
		{ 2.0f / width, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 2.0f / height, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f / (far - near), 0.0f },
		{ 0.0f, 0.0f, near / (near - far), 1.0f }
	};
	return Matrix4x4(temp);
}
Matrix4x4 Math::perspectiveFOV(float fovY, float width, float height, float near, float far)
{
	float yScale = Math::cot(fovY / 2.0f);
	float xScale = yScale * height / width;
	float temp[4][4] =
	{
		{ xScale, 0.0f, 0.0f, 0.0f },
		{ 0.0f, yScale, 0.0f, 0.0f },
		{ 0.0f, 0.0f, far / (far - near), 1.0f },
		{ 0.0f, 0.0f, -near * far / (far - near), 0.0f }
	};
	return Matrix4x4(temp);
}
Matrix4x4 Math::createSimpleViewProjectionMatrix(float width, float height)
{
	float temp[4][4] =
	{
		{ 2.0f / width, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 2.0f / height, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f, 1.0f }
	};
	return Matrix4x4(temp);
}