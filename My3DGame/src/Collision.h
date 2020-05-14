//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include <vector>
#include "Math/Math.h"

struct LineSegment
{
	LineSegment(const Vector3& start, const Vector3& end);
	// Get point along segment where 0 <= t <= 1
	Vector3 pointOnSegment(float t) const;
	// Get minimum distance squared between point and line segment
	float minDistSq(const Vector3& point) const;
	// Get MinDistSq between two line segments
	static float minDistSq(const LineSegment& s1, const LineSegment& s2);
	Vector3 start_;
	Vector3 end_;
};
struct Plane
{
	Plane(const Vector3& normal, float d);
	// Construct plane from three points
	Plane(const Vector3& a, const Vector3& b, const Vector3& c);
	// Get the signed distance between the point and the plane
	float signedDist(const Vector3& point) const;
	Vector3 normal_;
	float d_;
};
struct Sphere
{
	Sphere(const Vector3& center, float radius);
	bool contains(const Vector3& point) const;
	Vector3 center_;
	float radius_;
};
struct AABB
{
	AABB(const Vector3& min, const Vector3& max);
	//Update min and max accounting for this point (used when loading a model)
	void updateMinMax(const Vector3& point);
	//Rotated by a quaternion
	void rotate(const Quaternion& q);
	bool contains(const Vector3& point) const;
	float minDistSq(const Vector3& point) const;
	Vector3 min_;
	Vector3 max_;
};
struct OBB
{
	Vector3 center_;
	Quaternion rotation_;
	Vector3 extents_;
};
struct Capsule
{
	Capsule(const Vector3& start, const Vector3& end, float radius);
	//Get point along segment where 0 <= t <= 1
	Vector3 pointOnSegment(float t) const;
	bool contains(const Vector3& point) const;
	LineSegment segment_;
	float radius_;
};
struct ConvexPolygon
{
	bool contains(const Vector2& point) const;
	//Vertices have a clockwise ordering
	std::vector<Vector2> vertices_;
};
//Intersection functions
bool intersect(const Sphere& a, const Sphere& b);
bool intersect(const AABB& a, const AABB& b);
bool intersect(const Capsule& a, const Capsule& b);
bool intersect(const Sphere& s, const AABB& box);
bool intersect(const LineSegment& l, const Sphere& s, float& outT);
bool intersect(const LineSegment& l, const Plane& p, float& outT);
bool intersect(const LineSegment& l, const AABB& b, float& outT, Vector3& outNorm);
bool sweptSphere(const Sphere& P0, const Sphere& P1, const Sphere& Q0, const Sphere& Q1, float& t);