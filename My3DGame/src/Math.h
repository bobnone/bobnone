// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <limits>

namespace Math
{
	const float Pi = 3.1415926535f;
	const float TwoPi = Pi * 2.0f;
	const float PiOver2 = Pi / 2.0f;
	const float Infinity = std::numeric_limits<float>::infinity();
	const float NegInfinity = -std::numeric_limits<float>::infinity();

	inline float ToRadians(float degrees)
	{
		return degrees * Pi / 180.0f;
	}
	inline float ToDegrees(float radians)
	{
		return radians * 180.0f / Pi;
	}
	inline bool NearZero(float val, float epsilon = 0.001f)
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
	template <typename T>
	T Max(const T& a, const T& b)
	{
		return (a < b ? b : a);
	}
	template <typename T>
	T Min(const T& a, const T& b)
	{
		return (a < b ? a : b);
	}
	template <typename T>
	T Clamp(const T& value, const T& lower, const T& upper)
	{
		return Min(upper, Max(lower, value));
	}
	inline float Abs(float value)
	{
		return (float)fabs(value);
	}
	inline float Cos(float angle)
	{
		return cosf(angle);
	}
	inline float Sin(float angle)
	{
		return sinf(angle);
	}
	inline float Tan(float angle)
	{
		return tanf(angle);
	}
	inline float Acos(float value)
	{
		return acosf(value);
	}
	inline float Atan2(float y, float x)
	{
		return atan2f(y, x);
	}
	inline float Cot(float angle)
	{
		return 1.0f / Tan(angle);
	}
	inline float Lerp(float a, float b, float f)
	{
		return a + f * (b - a);
	}
	inline float Sqrt(float value)
	{
		return sqrtf(value);
	}
	inline float Fmod(float numer, float denom)
	{
		return (float)fmod(numer, denom);
	}
}
// 2D Vector
class vector2
{
public:
	float x;
	float y;
	vector2():x(0.0f), y(0.0f)
	{
		//
	}
	explicit vector2(float inX, float inY):x(inX), y(inY)
	{
		//
	}
	const float* GetAsFloatPtr() const
	{
		return reinterpret_cast<const float*>(&x);
	}
	// Set both components in one line
	void Set(float inX, float inY)
	{
		x = inX;
		y = inY;
	}
	// Vector addition (a + b)
	friend vector2 operator+(const vector2& a, const vector2& b)
	{
		return vector2(a.x + b.x, a.y + b.y);
	}
	// Vector subtraction (a - b)
	friend vector2 operator-(const vector2& a, const vector2& b)
	{
		return vector2(a.x - b.x, a.y - b.y);
	}
	// Component-wise multiplication
	// (a.x * b.x, ...)
	friend vector2 operator*(const vector2& a, const vector2& b)
	{
		return vector2(a.x * b.x, a.y * b.y);
	}
	// Scalar multiplication
	friend vector2 operator*(const vector2& vec, float scalar)
	{
		return vector2(vec.x * scalar, vec.y * scalar);
	}
	// Scalar multiplication
	friend vector2 operator*(float scalar, const vector2& vec)
	{
		return vector2(vec.x * scalar, vec.y * scalar);
	}
	// Scalar *=
	vector2& operator*=(float scalar)
	{
		x *= scalar;
		y *= scalar;
		return *this;
	}
	// Vector +=
	vector2& operator+=(const vector2& right)
	{
		x += right.x;
		y += right.y;
		return *this;
	}
	// Vector -=
	vector2& operator-=(const vector2& right)
	{
		x -= right.x;
		y -= right.y;
		return *this;
	}
	// Length squared of vector
	float Length2() const
	{
		return (x*x + y * y);
	}
	// Length of vector
	float Length() const
	{
		return (Math::Sqrt(Length2()));
	}
	// Normalize this vector
	void Normalize()
	{
		float length = Length();
		x /= length;
		y /= length;
	}
	// Normalize the provided vector
	static vector2 Normalize(const vector2& vec)
	{
		vector2 temp = vec;
		temp.Normalize();
		return temp;
	}
	// Dot product between two vectors (a dot b)
	static float Dot(const vector2& a, const vector2& b)
	{
		return (a.x * b.x + a.y * b.y);
	}
	// Lerp from A to B by f
	static vector2 Lerp(const vector2& a, const vector2& b, float f)
	{
		return vector2(a + f * (b - a));
	}
	// Reflect V about (normalized) N
	static vector2 Reflect(const vector2& v, const vector2& n)
	{
		return v - 2.0f * vector2::Dot(v, n) * n;
	}
	// Transform vector by matrix
	static vector2 Transform(const vector2& v, const class matrix3& m, float w = 1.0f);
	static const vector2 Zero;
	static const vector2 UnitX;
	static const vector2 UnitY;
	static const vector2 NegUnitX;
	static const vector2 NegUnitY;
};
// 3D Vector
class vector3
{
public:
	float x;
	float y;
	float z;
	vector3():x(0.0f), y(0.0f), z(0.0f)
	{
		//
	}
	explicit vector3(float inX, float inY, float inZ):x(inX), y(inY), z(inZ)
	{
		//
	}
	explicit vector3(vector2 v, float inZ) :x(v.x), y(v.y), z(inZ)
	{
		//
	}
	// Cast to a const float pointer
	const float* GetAsFloatPtr() const
	{
		return reinterpret_cast<const float*>(&x);
	}
	// Set all three components in one line
	void Set(float inX, float inY, float inZ)
	{
		x = inX;
		y = inY;
		z = inZ;
	}
	// Vector addition (a + b)
	friend vector3 operator+(const vector3& a, const vector3& b)
	{
		return vector3(a.x + b.x, a.y + b.y, a.z + b.z);
	}
	// Vector subtraction (a - b)
	friend vector3 operator-(const vector3& a, const vector3& b)
	{
		return vector3(a.x - b.x, a.y - b.y, a.z - b.z);
	}
	// Component-wise multiplication
	friend vector3 operator*(const vector3& left, const vector3& right)
	{
		return vector3(left.x * right.x, left.y * right.y, left.z * right.z);
	}
	// Scalar multiplication
	friend vector3 operator*(const vector3& vec, float scalar)
	{
		return vector3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
	}
	// Scalar multiplication
	friend vector3 operator*(float scalar, const vector3& vec)
	{
		return vector3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
	}
	// Scalar *=
	vector3& operator*=(float scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}
	// Vector +=
	vector3& operator+=(const vector3& right)
	{
		x += right.x;
		y += right.y;
		z += right.z;
		return *this;
	}
	// Vector -=
	vector3& operator-=(const vector3& right)
	{
		x -= right.x;
		y -= right.y;
		z -= right.z;
		return *this;
	}
	// Length squared of vector
	float Length2() const
	{
		return (x*x + y * y + z * z);
	}
	// Length of vector
	float Length() const
	{
		return (Math::Sqrt(Length2()));
	}
	// Normalize this vector
	void Normalize()
	{
		float length = Length();
		x /= length;
		y /= length;
		z /= length;
	}
	// Normalize the provided vector
	static vector3 Normalize(const vector3& vec)
	{
		vector3 temp = vec;
		temp.Normalize();
		return temp;
	}
	// Dot product between two vectors (a dot b)
	static float Dot(const vector3& a, const vector3& b)
	{
		return (a.x * b.x + a.y * b.y + a.z * b.z);
	}
	// Cross product between two vectors (a cross b)
	static vector3 Cross(const vector3& a, const vector3& b)
	{
		vector3 temp;
		temp.x = a.y * b.z - a.z * b.y;
		temp.y = a.z * b.x - a.x * b.z;
		temp.z = a.x * b.y - a.y * b.x;
		return temp;
	}
	// Lerp from A to B by f
	static vector3 Lerp(const vector3& a, const vector3& b, float f)
	{
		return vector3(a + f * (b - a));
	}
	// Reflect V about (normalized) N
	static vector3 Reflect(const vector3& v, const vector3& n)
	{
		return v - 2.0f * vector3::Dot(v, n) * n;
	}
	static vector3 Transform(const vector3& v, const class matrix4& m, float w = 1.0f);
	// This will transform the vector and renormalize the w component
	static vector3 TransformWithPerspDiv(const vector3& v, const class matrix4& m, float w = 1.0f);
	// Transform a Vector3 by a quaternion
	static vector3 Transform(const vector3& v, const class quaternion& q);
	static const vector3 Zero;
	static const vector3 UnitX;
	static const vector3 UnitY;
	static const vector3 UnitZ;
	static const vector3 NegUnitX;
	static const vector3 NegUnitY;
	static const vector3 NegUnitZ;
	static const vector3 Infinity;
	static const vector3 NegInfinity;
};
// 3x3 Matrix
class matrix3
{
public:
	float matrix[3][3] =
	{
		{1.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 1.0f}
	};
	matrix3()
	{
		//
	}
	explicit matrix3(float inputMatrix[3][3])
	{
		memcpy(matrix, inputMatrix, 9 * sizeof(float));
	}
	// Cast to a const float pointer
	const float* GetAsFloatPtr() const
	{
		return reinterpret_cast<const float*>(&matrix[0][0]);
	}
	// Matrix multiplication
	friend matrix3 operator*(const matrix3& left, const matrix3& right)
	{
		matrix3 retVal;
		// row 0
		retVal.matrix[0][0] = left.matrix[0][0] * right.matrix[0][0] + left.matrix[0][1] * right.matrix[1][0] + left.matrix[0][2] * right.matrix[2][0];
		retVal.matrix[0][1] = left.matrix[0][0] * right.matrix[0][1] + left.matrix[0][1] * right.matrix[1][1] + left.matrix[0][2] * right.matrix[2][1];
		retVal.matrix[0][2] = left.matrix[0][0] * right.matrix[0][2] + left.matrix[0][1] * right.matrix[1][2] + left.matrix[0][2] * right.matrix[2][2];
		// row 1
		retVal.matrix[1][0] = left.matrix[1][0] * right.matrix[0][0] + left.matrix[1][1] * right.matrix[1][0] + left.matrix[1][2] * right.matrix[2][0];
		retVal.matrix[1][1] = left.matrix[1][0] * right.matrix[0][1] + left.matrix[1][1] * right.matrix[1][1] + left.matrix[1][2] * right.matrix[2][1];
		retVal.matrix[1][2] = left.matrix[1][0] * right.matrix[0][2] + left.matrix[1][1] * right.matrix[1][2] + left.matrix[1][2] * right.matrix[2][2];
		// row 2
		retVal.matrix[2][0] = left.matrix[2][0] * right.matrix[0][0] + left.matrix[2][1] * right.matrix[1][0] + left.matrix[2][2] * right.matrix[2][0];
		retVal.matrix[2][1] = left.matrix[2][0] * right.matrix[0][1] + left.matrix[2][1] * right.matrix[1][1] + left.matrix[2][2] * right.matrix[2][1];
		retVal.matrix[2][2] = left.matrix[2][0] * right.matrix[0][2] + left.matrix[2][1] * right.matrix[1][2] + left.matrix[2][2] * right.matrix[2][2];
		return retVal;
	}
	matrix3& operator*=(const matrix3& right)
	{
		*this = *this * right;
		return *this;
	}
	// Create a scale matrix with x and y scales
	static matrix3 CreateScale(float xScale, float yScale)
	{
		float temp[3][3] =
		{
			{ xScale, 0.0f, 0.0f },
			{ 0.0f, yScale, 0.0f },
			{ 0.0f, 0.0f, 1.0f },
		};
		return matrix3(temp);
	}
	static matrix3 CreateScale(const vector2& scaleVector)
	{
		return CreateScale(scaleVector.x, scaleVector.y);
	}
	// Create a scale matrix with a uniform factor
	static matrix3 CreateScale(float scale)
	{
		return CreateScale(scale, scale);
	}
	// Create a rotation matrix about the Z axis
	// theta is in radians
	static matrix3 CreateRotation(float theta)
	{
		float temp[3][3] =
		{
			{ Math::Cos(theta), Math::Sin(theta), 0.0f },
			{ -Math::Sin(theta), Math::Cos(theta), 0.0f },
			{ 0.0f, 0.0f, 1.0f },
		};
		return matrix3(temp);
	}
	// Create a translation matrix (on the XY-plane)
	static matrix3 CreateTranslation(const vector2& trans)
	{
		float temp[3][3] =
		{
			{ 1.0f, 0.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f },
			{ trans.x, trans.y, 1.0f },
		};
		return matrix3(temp);
	}
};
// 4x4 Matrix
class matrix4
{
public:
	float matrix[4][4] =
	{
		{ 1.0f, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f }
	};
	matrix4()
	{
		//
	}
	explicit matrix4(float inMat[4][4])
	{
		memcpy(matrix, inMat, 16 * sizeof(float));
	}
	// Cast to a const float pointer
	const float* GetAsFloatPtr() const
	{
		return reinterpret_cast<const float*>(&matrix[0][0]);
	}
	// Matrix multiplication (a * b)
	friend matrix4 operator*(const matrix4& a, const matrix4& b)
	{
		matrix4 retVal;
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
	matrix4& operator*=(const matrix4& right)
	{
		*this = *this * right;
		return *this;
	}
	// Invert the matrix - super slow
	void Invert();
	// Get the translation component of the matrix
	vector3 GetTranslation() const
	{
		return vector3(matrix[3][0], matrix[3][1], matrix[3][2]);
	}
	// Get the X axis of the matrix (right)
	vector3 GetXAxis() const
	{
		return vector3::Normalize(vector3(matrix[0][0], matrix[0][1], matrix[0][2]));
	}
	// Get the Y axis of the matrix (up)
	vector3 GetYAxis() const
	{
		return vector3::Normalize(vector3(matrix[1][0], matrix[1][1], matrix[1][2]));
	}
	// Get the Z axis of the matrix (forward)
	vector3 GetZAxis() const
	{
		return vector3::Normalize(vector3(matrix[2][0], matrix[2][1], matrix[2][2]));
	}
	// Extract the scale component from the matrix
	vector3 GetScale() const
	{
		vector3 retVal;
		retVal.x = vector3(matrix[0][0], matrix[0][1], matrix[0][2]).Length();
		retVal.y = vector3(matrix[1][0], matrix[1][1], matrix[1][2]).Length();
		retVal.z = vector3(matrix[2][0], matrix[2][1], matrix[2][2]).Length();
		return retVal;
	}
	// Create a scale matrix with x, y, and z scales
	static matrix4 CreateScale(float xScale, float yScale, float zScale)
	{
		float temp[4][4] =
		{
			{ xScale, 0.0f, 0.0f, 0.0f },
			{ 0.0f, yScale, 0.0f, 0.0f },
			{ 0.0f, 0.0f, zScale, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f }
		};
		return matrix4(temp);
	}
	static matrix4 CreateScale(const vector3& scaleVector)
	{
		return CreateScale(scaleVector.x, scaleVector.y, scaleVector.z);
	}
	// Create a scale matrix with a uniform factor
	static matrix4 CreateScale(float scale)
	{
		return CreateScale(scale, scale, scale);
	}
	// Rotation about x-axis
	static matrix4 CreateRotationX(float theta)
	{
		float temp[4][4] =
		{
			{ 1.0f, 0.0f, 0.0f , 0.0f },
			{ 0.0f, Math::Cos(theta), Math::Sin(theta), 0.0f },
			{ 0.0f, -Math::Sin(theta), Math::Cos(theta), 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f },
		};
		return matrix4(temp);
	}
	// Rotation about y-axis
	static matrix4 CreateRotationY(float theta)
	{
		float temp[4][4] =
		{
			{ Math::Cos(theta), 0.0f, -Math::Sin(theta), 0.0f },
			{ 0.0f, 1.0f, 0.0f, 0.0f },
			{ Math::Sin(theta), 0.0f, Math::Cos(theta), 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f },
		};
		return matrix4(temp);
	}
	// Rotation about z-axis
	static matrix4 CreateRotationZ(float theta)
	{
		float temp[4][4] =
		{
			{ Math::Cos(theta), Math::Sin(theta), 0.0f, 0.0f },
			{ -Math::Sin(theta), Math::Cos(theta), 0.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f },
		};
		return matrix4(temp);
	}
	// Create a rotation matrix from a quaternion
	static matrix4 CreateFromQuaternion(const class quaternion& q);
	static matrix4 CreateTranslation(const vector3& trans)
	{
		float temp[4][4] =
		{
			{ 1.0f, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f, 0.0f },
			{ trans.x, trans.y, trans.z, 1.0f }
		};
		return matrix4(temp);
	}
	static matrix4 CreateLookAt(const vector3& eye, const vector3& target, const vector3& up)
	{
		vector3 zaxis = vector3::Normalize(target - eye);
		vector3 xaxis = vector3::Normalize(vector3::Cross(up, zaxis));
		vector3 yaxis = vector3::Normalize(vector3::Cross(zaxis, xaxis));
		vector3 trans;
		trans.x = -vector3::Dot(xaxis, eye);
		trans.y = -vector3::Dot(yaxis, eye);
		trans.z = -vector3::Dot(zaxis, eye);
		float temp[4][4] =
		{
			{ xaxis.x, yaxis.x, zaxis.x, 0.0f },
			{ xaxis.y, yaxis.y, zaxis.y, 0.0f },
			{ xaxis.z, yaxis.z, zaxis.z, 0.0f },
			{ trans.x, trans.y, trans.z, 1.0f }
		};
		return matrix4(temp);
	}
	static matrix4 CreateOrtho(float width, float height, float near, float far)
	{
		float temp[4][4] =
		{
			{ 2.0f / width, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 2.0f / height, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f / (far - near), 0.0f },
			{ 0.0f, 0.0f, near / (near - far), 1.0f }
		};
		return matrix4(temp);
	}
	static matrix4 CreatePerspectiveFOV(float fovY, float width, float height, float near, float far)
	{
		float yScale = Math::Cot(fovY / 2.0f);
		float xScale = yScale * height / width;
		float temp[4][4] =
		{
			{ xScale, 0.0f, 0.0f, 0.0f },
			{ 0.0f, yScale, 0.0f, 0.0f },
			{ 0.0f, 0.0f, far / (far - near), 1.0f },
			{ 0.0f, 0.0f, -near * far / (far - near), 0.0f }
		};
		return matrix4(temp);
	}

	// Create "Simple" View-Projection Matrix from Chapter 6
	static matrix4 CreateSimpleViewProj(float width, float height)
	{
		float temp[4][4] =
		{
			{ 2.0f / width, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 2.0f / height, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f, 1.0f }
		};
		return matrix4(temp);
	}
};
// (Unit) Quaternion
class quaternion
{
public:
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 1.0f;
	quaternion()
	{
		//
	}
	// This directly sets the quaternion components --
	// don't use for axis/angle
	explicit quaternion(float inX, float inY, float inZ, float inW)
	{
		Set(inX, inY, inZ, inW);
	}
	// Construct the quaternion from an axis and angle
	// It is assumed that axis is already normalized,
	// and the angle is in radians
	explicit quaternion(const vector3& axis, float angle)
	{
		float scalar = Math::Sin(angle / 2.0f);
		x = axis.x * scalar;
		y = axis.y * scalar;
		z = axis.z * scalar;
		w = Math::Cos(angle / 2.0f);
	}
	// Directly set the internal components
	void Set(float inX, float inY, float inZ, float inW)
	{
		x = inX;
		y = inY;
		z = inZ;
		w = inW;
	}
	void Conjugate()
	{
		x *= -1.0f;
		y *= -1.0f;
		z *= -1.0f;
	}
	float Length2() const
	{
		return (x*x + y * y + z * z + w * w);
	}
	float Length() const
	{
		return Math::Sqrt(Length2());
	}
	void Normalize()
	{
		float length = Length();
		x /= length;
		y /= length;
		z /= length;
		w /= length;
	}
	// Normalize the provided quaternion
	static quaternion Normalize(const quaternion& q)
	{
		quaternion retVal = q;
		retVal.Normalize();
		return retVal;
	}
	// Linear interpolation
	static quaternion Lerp(const quaternion& a, const quaternion& b, float f)
	{
		quaternion retVal;
		retVal.x = Math::Lerp(a.x, b.x, f);
		retVal.y = Math::Lerp(a.y, b.y, f);
		retVal.z = Math::Lerp(a.z, b.z, f);
		retVal.w = Math::Lerp(a.w, b.w, f);
		retVal.Normalize();
		return retVal;
	}
	static float Dot(const quaternion& a, const quaternion& b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	}
	// Spherical Linear Interpolation
	static quaternion Slerp(const quaternion& a, const quaternion& b, float f)
	{
		float rawCosm = quaternion::Dot(a, b);
		float cosom = -rawCosm;
		if (rawCosm >= 0.0f)
		{
			cosom = rawCosm;
		}
		float scale0, scale1;
		if (cosom < 0.9999f)
		{
			const float omega = Math::Acos(cosom);
			const float invSin = 1.f / Math::Sin(omega);
			scale0 = Math::Sin((1.f - f) * omega) * invSin;
			scale1 = Math::Sin(f * omega) * invSin;
		}
		else
		{
			// Use linear interpolation if the quaternions
			// are collinear
			scale0 = 1.0f - f;
			scale1 = f;
		}
		if (rawCosm < 0.0f)
		{
			scale1 = -scale1;
		}
		quaternion retVal;
		retVal.x = scale0 * a.x + scale1 * b.x;
		retVal.y = scale0 * a.y + scale1 * b.y;
		retVal.z = scale0 * a.z + scale1 * b.z;
		retVal.w = scale0 * a.w + scale1 * b.w;
		retVal.Normalize();
		return retVal;
	}
	// Concatenate
	// Rotate by q FOLLOWED BY p
	static quaternion Concatenate(const quaternion& q, const quaternion& p)
	{
		quaternion retVal;
		// Vector component is:
		// ps * qv + qs * pv + pv x qv
		vector3 qv(q.x, q.y, q.z);
		vector3 pv(p.x, p.y, p.z);
		vector3 newVec = p.w * qv + q.w * pv + vector3::Cross(pv, qv);
		retVal.x = newVec.x;
		retVal.y = newVec.y;
		retVal.z = newVec.z;
		// Scalar component is:
		// ps * qs - pv . qv
		retVal.w = p.w * q.w - vector3::Dot(pv, qv);
		return retVal;
	}
};