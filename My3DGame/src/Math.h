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
	const float PI = 3.1415926535f;
	const float TWO_PI = PI * 2.0f;
	const float PI_OVER_TWO = PI / 2.0f;
	const float INFINITE_FLOAT = std::numeric_limits<float>::infinity();
	const float NEGATIVE_INFINITE_FLOAT = -std::numeric_limits<float>::infinity();

	inline float toRadians(float degrees)
	{
		return degrees * PI / 180.0f;
	}
	inline float toDegrees(float radians)
	{
		return radians * 180.0f / PI;
	}
	inline bool nearZero(float val, float epsilon = 0.001f)
	{
		if(fabs(val) <= epsilon)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
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
		return Min(upper, Max(lower, value));
	}
	inline float abs(float value)
	{
		return (float)fabs(value);
	}
	inline float cos(float angle)
	{
		return cosf(angle);
	}
	inline float sin(float angle)
	{
		return sinf(angle);
	}
	inline float tan(float angle)
	{
		return tanf(angle);
	}
	inline float acos(float value)
	{
		return acosf(value);
	}
	inline float atan2(float y, float x)
	{
		return atan2f(y, x);
	}
	inline float cot(float angle)
	{
		return 1.0f / tan(angle);
	}
	inline float lerp(float a, float b, float f)
	{
		return a + f * (b - a);
	}
	inline float sqrt(float value)
	{
		return sqrtf(value);
	}
	inline float mod(float numer, float denom)
	{
		return (float)fmod(numer, denom);
	}
}
//2D Vector
class Vector2
{
public:
	float x;
	float y;
	Vector2():x(0.0f), y(0.0f)
	{
		//EMPTY:
	}
	explicit Vector2(float inX, float inY):x(inX), y(inY)
	{
		//EMPTY:
	}
	explicit Vector2(float inXY) :x(inXY), y(inXY)
	{
		//EMPTY:
	}
	const float* getAsFloatPtr() const
	{
		return reinterpret_cast<const float*>(&x);
	}
	//Set both components in one line
	void set(float inX, float inY)
	{
		x = inX;
		y = inY;
	}
	//Vector addition (a + b)
	friend Vector2 operator+(const Vector2& a, const Vector2& b)
	{
		return Vector2(a.x + b.x, a.y + b.y);
	}
	//Vector subtraction (a - b)
	friend Vector2 operator-(const Vector2& a, const Vector2& b)
	{
		return Vector2(a.x - b.x, a.y - b.y);
	}
	//Component-wise multiplication (a.x * b.x, ...)
	friend Vector2 operator*(const Vector2& a, const Vector2& b)
	{
		return Vector2(a.x * b.x, a.y * b.y);
	}
	//Scalar multiplication
	friend Vector2 operator*(const Vector2& vec, float scalar)
	{
		return Vector2(vec.x * scalar, vec.y * scalar);
	}
	//Scalar multiplication
	friend Vector2 operator*(float scalar, const Vector2& vec)
	{
		return Vector2(vec.x * scalar, vec.y * scalar);
	}
	//Scalar *=
	Vector2& operator*=(float scalar)
	{
		x *= scalar;
		y *= scalar;
		return *this;
	}
	//Vector ==
	bool operator==(const Vector2& right)
	{
		if (x == right.x)
		{
			if (y == right.y)
			{
				return true;
			}
			return false;
		}
		return false;
	}
	//Vector !=
	bool operator!=(const Vector2& right)
	{
		if (x != right.x)
		{
			if (y != right.y)
			{
				return true;
			}
			return false;
		}
		return false;
	}
	//Vector +=
	Vector2& operator+=(const Vector2& right)
	{
		x += right.x;
		y += right.y;
		return *this;
	}
	//Vector -=
	Vector2& operator-=(const Vector2& right)
	{
		x -= right.x;
		y -= right.y;
		return *this;
	}
	//Length squared of vector
	float length2() const
	{
		return (x * x + y * y);
	}
	//Length of vector
	float length() const
	{
		return (Math::sqrt(length2()));
	}
	//Normalize this vector
	void normalize()
	{
		float l = length();
		x /= l;
		y /= l;
	}
	//Normalize the provided vector
	static Vector2 normalize(const Vector2& vec)
	{
		Vector2 temp = vec;
		temp.normalize();
		return temp;
	}
	//Dot product between two vectors (a dot b)
	static float dot(const Vector2& a, const Vector2& b)
	{
		return (a.x * b.x + a.y * b.y);
	}
	//Lerp from A to B by f
	static Vector2 lerp(const Vector2& a, const Vector2& b, float f)
	{
		return Vector2(a + f * (b - a));
	}
	//Reflect V about (normalized) N
	static Vector2 reflect(const Vector2& v, const Vector2& n)
	{
		return v - 2.0f * Vector2::dot(v, n) * n;
	}
	//Transform a Vector2 by Matrix3
	static Vector2 transform(const Vector2& v, const class Matrix3& m, float w = 1.0f);
	//Constants:
	static const Vector2 UNIT_X;
	static const Vector2 UNIT_Y;
	static const Vector2 NEGATIVE_UNIT_X;
	static const Vector2 NEGATIVE_UNIT_Y;
	static const Vector2 INFINITE_VECTOR;
	static const Vector2 NEGATIVE_INFINITE_VECTOR;
};
//3D Vector
class Vector3
{
public:
	float x;
	float y;
	float z;
	Vector3():x(0.0f), y(0.0f), z(0.0f)
	{
		//EMPTY:
	}
	explicit Vector3(float inX, float inY, float inZ):x(inX), y(inY), z(inZ)
	{
		//EMPTY:
	}
	explicit Vector3(Vector2 v, float inZ):x(v.x), y(v.y), z(inZ)
	{
		//EMPTY:
	}
	explicit Vector3(float inXYZ) :x(inXYZ), y(inXYZ), z(inXYZ)
	{
		//EMPTY:
	}
	//Cast to a const float pointer
	const float* getAsFloatPtr() const
	{
		return reinterpret_cast<const float*>(&x);
	}
	//Set all three components in one line
	void set(float inX, float inY, float inZ)
	{
		x = inX;
		y = inY;
		z = inZ;
	}
	//Vector addition (a + b)
	friend Vector3 operator+(const Vector3& a, const Vector3& b)
	{
		return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
	}
	//Vector subtraction (a - b)
	friend Vector3 operator-(const Vector3& a, const Vector3& b)
	{
		return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
	}
	//Component-wise multiplication
	friend Vector3 operator*(const Vector3& left, const Vector3& right)
	{
		return Vector3(left.x * right.x, left.y * right.y, left.z * right.z);
	}
	//Component-wise division
	friend Vector3 operator/(const Vector3& left, const Vector3& right)
	{
		return Vector3(left.x / right.x, left.y / right.y, left.z / right.z);
	}
	//Scalar multiplication
	friend Vector3 operator*(const Vector3& vec, float scalar)
	{
		return Vector3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
	}
	//Scalar multiplication
	friend Vector3 operator*(float scalar, const Vector3& vec)
	{
		return Vector3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
	}
	//Scalar *=
	Vector3& operator*=(float scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}
	//Vector ==
	bool operator==(const Vector3& right)
	{
		if(x == right.x)
		{
			if(y == right.y)
			{
				if(z == right.z)
				{
					return true;
				}
				return false;
			}
			return false;
		}
		return false;
	}
	//Vector !=
	bool operator!=(const Vector3& right)
	{
		if(x != right.x)
		{
			if(y != right.y)
			{
				if(z != right.z)
				{
					return true;
				}
				return false;
			}
			return false;
		}
		return false;
	}
	//Vector +=
	Vector3& operator+=(const Vector3& right)
	{
		x += right.x;
		y += right.y;
		z += right.z;
		return *this;
	}
	//Vector -=
	Vector3& operator-=(const Vector3& right)
	{
		x -= right.x;
		y -= right.y;
		z -= right.z;
		return *this;
	}
	//Length squared of vector
	float length2() const
	{
		return (x * x + y * y + z * z);
	}
	//Length of vector
	float length() const
	{
		return (Math::sqrt(length2()));
	}
	//Normalize this vector
	void normalize()
	{
		float l = length();
		x /= l;
		y /= l;
		z /= l;
	}
	//Normalize the provided vector
	static Vector3 normalize(const Vector3& vec)
	{
		Vector3 temp = vec;
		temp.normalize();
		return temp;
	}
	//Dot product between two vectors (a dot b)
	static float dot(const Vector3& a, const Vector3& b)
	{
		return (a.x * b.x + a.y * b.y + a.z * b.z);
	}
	//Cross product between two vectors (a cross b)
	static Vector3 cross(const Vector3& a, const Vector3& b)
	{
		Vector3 temp;
		temp.x = a.y * b.z - a.z * b.y;
		temp.y = a.z * b.x - a.x * b.z;
		temp.z = a.x * b.y - a.y * b.x;
		return temp;
	}
	//Lerp from A to B by f
	static Vector3 lerp(const Vector3& a, const Vector3& b, float f)
	{
		return Vector3(a + f * (b - a));
	}
	//Reflect V about (normalized) N
	static Vector3 reflect(const Vector3& v, const Vector3& n)
	{
		return v - 2.0f * Vector3::dot(v, n) * n;
	}
	//Transform a Vector3 by a Matrix4
	static Vector3 transform(const Vector3& v, const class Matrix4& m, float w = 1.0f);
	//This will transform the vector and renormalize the w component
	static Vector3 transformWithPerspDiv(const Vector3& v, const class Matrix4& m, float w = 1.0f);
	//Transform a Vector3 by a Quaternion
	static Vector3 transform(const Vector3& v, const class Quaternion& q);
	//Constants:
	static const Vector3 UNIT_X;
	static const Vector3 UNIT_Y;
	static const Vector3 UNIT_Z;
	static const Vector3 NEGATIVE_UNIT_X;
	static const Vector3 NEGATIVE_UNIT_Y;
	static const Vector3 NEGATIVE_UNIT_Z;
	static const Vector3 INFINITE_VECTOR;
	static const Vector3 NEGATIVE_INFINITE_VECTOR;
};
//3x3 Matrix
class Matrix3
{
public:
	float matrix[3][3] =
	{
		{1.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 1.0f}
	};
	Matrix3()
	{
		//EMPTY:
	}
	explicit Matrix3(float inputMatrix[3][3])
	{
		memcpy(matrix, inputMatrix, 9 * sizeof(float));
	}
	//Cast to a const float pointer
	const float* getAsFloatPtr() const
	{
		return reinterpret_cast<const float*>(&matrix[0][0]);
	}
	//Matrix multiplication
	friend Matrix3 operator*(const Matrix3& left, const Matrix3& right)
	{
		Matrix3 retVal;
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
	Matrix3& operator*=(const Matrix3& right)
	{
		*this = *this * right;
		return *this;
	}
	//Create a scale matrix with x and y scales
	static Matrix3 createScale(float xScale, float yScale)
	{
		float temp[3][3] =
		{
			{ xScale, 0.0f, 0.0f },
			{ 0.0f, yScale, 0.0f },
			{ 0.0f, 0.0f, 1.0f },
		};
		return Matrix3(temp);
	}
	static Matrix3 createScale(const Vector2& scaleVector)
	{
		return createScale(scaleVector.x, scaleVector.y);
	}
	//Create a scale matrix with a uniform factor
	static Matrix3 createScale(float scale)
	{
		return createScale(scale, scale);
	}
	//Create a rotation matrix about the Z axis (theta is in radians)
	static Matrix3 createRotation(float theta)
	{
		float temp[3][3] =
		{
			{ Math::cos(theta), Math::sin(theta), 0.0f },
			{ -Math::sin(theta), Math::cos(theta), 0.0f },
			{ 0.0f, 0.0f, 1.0f },
		};
		return Matrix3(temp);
	}
	//Create a translation matrix (on the XY-plane)
	static Matrix3 createTranslation(const Vector2& trans)
	{
		float temp[3][3] =
		{
			{ 1.0f, 0.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f },
			{ trans.x, trans.y, 1.0f },
		};
		return Matrix3(temp);
	}
};
//4x4 Matrix
class Matrix4
{
public:
	float matrix[4][4] =
	{
		{ 1.0f, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f }
	};
	Matrix4()
	{
		//EMPTY:
	}
	explicit Matrix4(float inputMatrix[4][4])
	{
		memcpy(matrix, inputMatrix, 16 * sizeof(float));
	}
	//Cast to a const float pointer
	const float* getAsFloatPtr() const
	{
		return reinterpret_cast<const float*>(&matrix[0][0]);
	}
	//Matrix multiplication (a * b)
	friend Matrix4 operator*(const Matrix4& a, const Matrix4& b)
	{
		Matrix4 retVal;
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
	Matrix4& operator*=(const Matrix4& right)
	{
		*this = *this * right;
		return *this;
	}
	//Invert the matrix - super slow
	void invert();
	//Get the translation component of the matrix
	Vector3 getTranslation() const
	{
		return Vector3(matrix[3][0], matrix[3][1], matrix[3][2]);
	}
	//Get the X axis of the matrix (right)
	Vector3 getXAxis() const
	{
		return Vector3::normalize(Vector3(matrix[0][0], matrix[0][1], matrix[0][2]));
	}
	//Get the Y axis of the matrix (up)
	Vector3 getYAxis() const
	{
		return Vector3::normalize(Vector3(matrix[1][0], matrix[1][1], matrix[1][2]));
	}
	//Get the Z axis of the matrix (forward)
	Vector3 getZAxis() const
	{
		return Vector3::normalize(Vector3(matrix[2][0], matrix[2][1], matrix[2][2]));
	}
	//Extract the scale component from the matrix
	Vector3 getScale() const
	{
		Vector3 retVal;
		retVal.x = Vector3(matrix[0][0], matrix[0][1], matrix[0][2]).length();
		retVal.y = Vector3(matrix[1][0], matrix[1][1], matrix[1][2]).length();
		retVal.z = Vector3(matrix[2][0], matrix[2][1], matrix[2][2]).length();
		return retVal;
	}
	//Create a scale matrix with x, y, and z scales
	static Matrix4 createScale(float xScale, float yScale, float zScale)
	{
		float temp[4][4] =
		{
			{ xScale, 0.0f, 0.0f, 0.0f },
			{ 0.0f, yScale, 0.0f, 0.0f },
			{ 0.0f, 0.0f, zScale, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f }
		};
		return Matrix4(temp);
	}
	static Matrix4 createScale(const Vector3& scaleVector)
	{
		return createScale(scaleVector.x, scaleVector.y, scaleVector.z);
	}
	//Create a scale matrix with a uniform factor
	static Matrix4 createScale(float scale)
	{
		return createScale(scale, scale, scale);
	}
	//Rotation about x-axis
	static Matrix4 createRotationX(float theta)
	{
		float temp[4][4] =
		{
			{ 1.0f, 0.0f, 0.0f , 0.0f },
			{ 0.0f, Math::cos(theta), Math::sin(theta), 0.0f },
			{ 0.0f, -Math::sin(theta), Math::cos(theta), 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f },
		};
		return Matrix4(temp);
	}
	//Rotation about y-axis
	static Matrix4 createRotationY(float theta)
	{
		float temp[4][4] =
		{
			{ Math::cos(theta), 0.0f, -Math::sin(theta), 0.0f },
			{ 0.0f, 1.0f, 0.0f, 0.0f },
			{ Math::sin(theta), 0.0f, Math::cos(theta), 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f },
		};
		return Matrix4(temp);
	}
	//Rotation about z-axis
	static Matrix4 CreateRotationZ(float theta)
	{
		float temp[4][4] =
		{
			{ Math::cos(theta), Math::sin(theta), 0.0f, 0.0f },
			{ -Math::sin(theta), Math::cos(theta), 0.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f },
		};
		return Matrix4(temp);
	}
	//Create a rotation matrix from a quaternion
	static Matrix4 fromQuaternion(const class Quaternion& q);
	static Matrix4 createTranslation(const Vector3& trans)
	{
		float temp[4][4] =
		{
			{ 1.0f, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f, 0.0f },
			{ trans.x, trans.y, trans.z, 1.0f }
		};
		return Matrix4(temp);
	}
	static Matrix4 createLookAt(const Vector3& eye, const Vector3& target, const Vector3& up)
	{
		Vector3 zaxis = Vector3::normalize(target - eye);
		Vector3 xaxis = Vector3::normalize(Vector3::cross(up, zaxis));
		Vector3 yaxis = Vector3::normalize(Vector3::cross(zaxis, xaxis));
		Vector3 trans;
		trans.x = -Vector3::dot(xaxis, eye);
		trans.y = -Vector3::dot(yaxis, eye);
		trans.z = -Vector3::dot(zaxis, eye);
		float temp[4][4] =
		{
			{ xaxis.x, yaxis.x, zaxis.x, 0.0f },
			{ xaxis.y, yaxis.y, zaxis.y, 0.0f },
			{ xaxis.z, yaxis.z, zaxis.z, 0.0f },
			{ trans.x, trans.y, trans.z, 1.0f }
		};
		return Matrix4(temp);
	}
	static Matrix4 createOrtho(float width, float height, float near, float far)
	{
		float temp[4][4] =
		{
			{ 2.0f / width, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 2.0f / height, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f / (far - near), 0.0f },
			{ 0.0f, 0.0f, near / (near - far), 1.0f }
		};
		return Matrix4(temp);
	}
	static Matrix4 createPerspectiveFOV(float fovY, float width, float height, float near, float far)
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
		return Matrix4(temp);
	}

	//Create "Simple" View-Projection Matrix from Chapter 6
	static Matrix4 createSimpleViewProj(float width, float height)
	{
		float temp[4][4] =
		{
			{ 2.0f / width, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 2.0f / height, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f, 1.0f }
		};
		return Matrix4(temp);
	}
};
//(Unit) Quaternion
class Quaternion
{
public:
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 1.0f;
	Quaternion()
	{
		//EMPTY:
	}
	// This directly sets the quaternion components -- don't use for axis/angle
	explicit Quaternion(float inX, float inY, float inZ, float inW)
	{
		set(inX, inY, inZ, inW);
	}
	//Construct the quaternion from an axis and angle
	//Note: It is assumed that axis is already normalized, and the angle is in radians
	explicit Quaternion(const Vector3& axis, float angle)
	{
		float scalar = Math::sin(angle / 2.0f);
		x = axis.x * scalar;
		y = axis.y * scalar;
		z = axis.z * scalar;
		w = Math::cos(angle / 2.0f);
	}
	//Directly set the internal components
	void set(float inX, float inY, float inZ, float inW)
	{
		x = inX;
		y = inY;
		z = inZ;
		w = inW;
	}
	void conjugate()
	{
		x *= -1.0f;
		y *= -1.0f;
		z *= -1.0f;
	}
	float length2() const
	{
		return (x * x + y * y + z * z + w * w);
	}
	float length() const
	{
		return Math::sqrt(length2());
	}
	void normalize()
	{
		float l = length();
		x /= l;
		y /= l;
		z /= l;
		w /= l;
	}
	//Normalize the provided quaternion
	static Quaternion normalize(const Quaternion& q)
	{
		Quaternion retVal = q;
		retVal.normalize();
		return retVal;
	}
	//Linear interpolation
	static Quaternion lerp(const Quaternion& a, const Quaternion& b, float f)
	{
		Quaternion retVal;
		retVal.x = Math::lerp(a.x, b.x, f);
		retVal.y = Math::lerp(a.y, b.y, f);
		retVal.z = Math::lerp(a.z, b.z, f);
		retVal.w = Math::lerp(a.w, b.w, f);
		retVal.normalize();
		return retVal;
	}
	static float dot(const Quaternion& a, const Quaternion& b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	}
	//Spherical Linear Interpolation
	static Quaternion slerp(const Quaternion& a, const Quaternion& b, float f)
	{
		float rawCosm = Quaternion::dot(a, b);
		float cosom = -rawCosm;
		if(rawCosm >= 0.0f)
		{
			cosom = rawCosm;
		}
		float scale0, scale1;
		if(cosom < 0.9999f)
		{
			const float omega = Math::acos(cosom);
			const float invSin = 1.f / Math::sin(omega);
			scale0 = Math::sin((1.f - f) * omega) * invSin;
			scale1 = Math::sin(f * omega) * invSin;
		}
		else
		{
			//Use linear interpolation if the quaternions are collinear
			scale0 = 1.0f - f;
			scale1 = f;
		}
		if(rawCosm < 0.0f)
		{
			scale1 = -scale1;
		}
		Quaternion retVal;
		retVal.x = scale0 * a.x + scale1 * b.x;
		retVal.y = scale0 * a.y + scale1 * b.y;
		retVal.z = scale0 * a.z + scale1 * b.z;
		retVal.w = scale0 * a.w + scale1 * b.w;
		retVal.normalize();
		return retVal;
	}
	//Concatenate - rotate by q followed BY p
	static Quaternion concatenate(const Quaternion& q, const Quaternion& p)
	{
		Quaternion retVal;
		//Vector component is:
		//ps * qv + qs * pv + pv x qv
		Vector3 qv(q.x, q.y, q.z);
		Vector3 pv(p.x, p.y, p.z);
		Vector3 newVec = p.w * qv + q.w * pv + Vector3::cross(pv, qv);
		retVal.x = newVec.x;
		retVal.y = newVec.y;
		retVal.z = newVec.z;
		//Scalar component is:
		//ps * qs - pv . qv
		retVal.w = p.w * q.w - Vector3::dot(pv, qv);
		return retVal;
	}
};