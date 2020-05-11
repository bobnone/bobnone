//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include "Math.h"
#include <vector>

struct LineSegment
{
	LineSegment(const vector3& start, const vector3& end);
	// Get point along segment where 0 <= t <= 1
	vector3 pointOnSegment(float t) const;
	// Get minimum distance squared between point and line segment
	float minDistSq(const vector3& point) const;
	// Get MinDistSq between two line segments
	static float minDistSq(const LineSegment& s1, const LineSegment& s2);
	vector3 start_;
	vector3 end_;
};
struct Plane
{
	Plane(const vector3& normal, float d);
	// Construct plane from three points
	Plane(const vector3& a, const vector3& b, const vector3& c);
	// Get the signed distance between the point and the plane
	float signedDist(const vector3& point) const;
	vector3 normal_;
	float d_;
};
struct Sphere
{
	Sphere(const vector3& center, float radius);
	bool contains(const vector3& point) const;
	vector3 center_;
	float radius_;
};
struct AABB
{
	AABB(const vector3& min, const vector3& max);
	//Update min and max accounting for this point (used when loading a model)
	void updateMinMax(const vector3& point);
	//Rotated by a quaternion
	void rotate(const quaternion& q);
	bool contains(const vector3& point) const;
	float minDistSq(const vector3& point) const;
	vector3 min_;
	vector3 max_;
};
struct OBB
{
	vector3 center_;
	quaternion rotation_;
	vector3 extents_;
};
struct Capsule
{
	Capsule(const vector3& start, const vector3& end, float radius);
	//Get point along segment where 0 <= t <= 1
	vector3 pointOnSegment(float t) const;
	bool contains(const vector3& point) const;
	LineSegment segment_;
	float radius_;
};
struct ConvexPolygon
{
	bool contains(const vector2& point) const;
	//Vertices have a clockwise ordering
	std::vector<vector2> vertices_;
};
//Intersection functions
bool intersect(const Sphere& a, const Sphere& b);
bool intersect(const AABB& a, const AABB& b);
bool intersect(const Capsule& a, const Capsule& b);
bool intersect(const Sphere& s, const AABB& box);
bool intersect(const LineSegment& l, const Sphere& s, float& outT);
bool intersect(const LineSegment& l, const Plane& p, float& outT);
bool intersect(const LineSegment& l, const AABB& b, float& outT, vector3& outNorm);
bool sweptSphere(const Sphere& P0, const Sphere& P1, const Sphere& Q0, const Sphere& Q1, float& t);