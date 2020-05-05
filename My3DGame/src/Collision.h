// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Math.h"
#include <vector>

struct LineSegment
{
	LineSegment(const vector3& start, const vector3& end);
	// Get point along segment where 0 <= t <= 1
	vector3 PointOnSegment(float t) const;
	// Get minimum distance squared between point and line segment
	float MinDistSq(const vector3& point) const;
	// Get MinDistSq between two line segments
	static float MinDistSq(const LineSegment& s1, const LineSegment& s2);
	vector3 mStart;
	vector3 mEnd;
};
struct Plane
{
	Plane(const vector3& normal, float d);
	// Construct plane from three points
	Plane(const vector3& a, const vector3& b, const vector3& c);
	// Get the signed distance between the point and the plane
	float SignedDist(const vector3& point) const;
	vector3 mNormal;
	float mD;
};
struct Sphere
{
	Sphere(const vector3& center, float radius);
	bool Contains(const vector3& point) const;
	vector3 mCenter;
	float mRadius;
};
struct AABB
{
	AABB(const vector3& min, const vector3& max);
	// Update min and max accounting for this point
	// (used when loading a model)
	void UpdateMinMax(const vector3& point);
	// Rotated by a quaternion
	void Rotate(const quaternion& q);
	bool Contains(const vector3& point) const;
	float MinDistSq(const vector3& point) const;
	vector3 mMin;
	vector3 mMax;
};
struct OBB
{
	vector3 mCenter;
	quaternion mRotation;
	vector3 mExtents;
};
struct Capsule
{
	Capsule(const vector3& start, const vector3& end, float radius);
	// Get point along segment where 0 <= t <= 1
	vector3 PointOnSegment(float t) const;
	bool Contains(const vector3& point) const;
	LineSegment mSegment;
	float mRadius;
};
struct ConvexPolygon
{
	bool Contains(const vector2& point) const;
	// Vertices have a clockwise ordering
	std::vector<vector2> mVertices;
};
// Intersection functions
bool Intersect(const Sphere& a, const Sphere& b);
bool Intersect(const AABB& a, const AABB& b);
bool Intersect(const Capsule& a, const Capsule& b);
bool Intersect(const Sphere& s, const AABB& box);
bool Intersect(const LineSegment& l, const Sphere& s, float& outT);
bool Intersect(const LineSegment& l, const Plane& p, float& outT);
bool Intersect(const LineSegment& l, const AABB& b, float& outT, vector3& outNorm);
bool SweptSphere(const Sphere& P0, const Sphere& P1, const Sphere& Q0, const Sphere& Q1, float& t);