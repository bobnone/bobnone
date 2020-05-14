//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------
//MinDistSq between two line segments:
//Copyright 2001 softSurfer, 2012 Dan Sunday
//This code may be freely used, distributed and modified for any purpose
//providing that this copyright notice is included with it.
//SoftSurfer makes no warranty for this code, and cannot be held
//liable for any real or imagined damage resulting from its use.
//Users of this code must verify correctness for their application.
//----------------------------------------------------------------

#include "Collision.h"
#include <algorithm>
#include <array>

LineSegment::LineSegment(const Vector3& start, const Vector3& end): start_(start), end_(end)
{
	//EMPTY:
}
Vector3 LineSegment::pointOnSegment(float t) const
{
	return start_ + (end_ - start_) * t;
}
float LineSegment::minDistSq(const Vector3& point) const
{
	//Construct vectors
	Vector3 ab = end_ - start_;
	Vector3 ba = -1.0f * ab;
	Vector3 ac = point - start_;
	Vector3 bc = point - end_;
	//Case 1: C projects prior to A
	if(Math::dot(ab, ac) < 0.0f)
	{
		return ac.length2();
	}
	//Case 2: C projects after B
	else if(Math::dot(ba, bc) < 0.0f)
	{
		return bc.length2();
	}
	//Case 3: C projects onto line
	else
	{
		//Compute p
		float scalar = Math::dot(ac, ab) / Math::dot(ab, ab);
		Vector3 p = scalar * ab;
		//Compute length squared of ac - p
		return (ac - p).length2();
	}
}
float LineSegment::minDistSq(const LineSegment & s1, const LineSegment & s2)
{
	Vector3   u = s1.end_ - s1.start_;
	Vector3   v = s2.end_ - s2.start_;
	Vector3   w = s1.start_ - s2.start_;
	//always >= 0
	float    a = Math::dot(u, u);
	float    b = Math::dot(u, v);
	//always >= 0
	float    c = Math::dot(v, v);
	float    d = Math::dot(u, w);
	float    e = Math::dot(v, w);
	//always >= 0
	float    D = a * c - b * b;
	//sc = sN / sD, default sD = D >= 0
	float    sc, sN, sD = D;
	//tc = tN / tD, default tD = D >= 0
	float    tc, tN, tD = D;
	//compute the line parameters of the two closest points
	//the lines are almost parallel
	if(Math::nearZero(D))
	{
		//force using point P0 on segment S1
		sN = 0.0;
		//to prevent possible division by 0.0 later
		sD = 1.0;
		tN = e;
		tD = c;
	}
	else//get the closest points on the infinite lines
	{
		sN = (b*e - c * d);
		tN = (a*e - b * d);
		//sc < 0 => the s=0 edge is visible
		if(sN < 0.0)
		{
			sN = 0.0;
			tN = e;
			tD = c;
		}
		else if(sN > sD)//sc > 1  => the s=1 edge is visible
		{
			sN = sD;
			tN = e + b;
			tD = c;
		}
	}
	//tc < 0 => the t=0 edge is visible
	if(tN < 0.0)
	{
		tN = 0.0;
		//recompute sc for this edge
		if(-d < 0.0)
		{
			sN = 0.0;
		}
		else if(-d > a)
		{
			sN = sD;
		}
		else
		{
			sN = -d;
			sD = a;
		}
	}
	else if(tN > tD)//tc > 1  => the t=1 edge is visible
	{
		tN = tD;
		//recompute sc for this edge
		if((-d + b) < 0.0f)
		{
			sN = 0;
		}
		else if((-d + b) > a)
		{
			sN = sD;
		}
		else
		{
			sN = (-d + b);
			sD = a;
		}
	}
	//finally do the division to get sc and tc
	sc = (Math::nearZero(sN) ? 0.0f : sN / sD);
	tc = (Math::nearZero(tN) ? 0.0f : tN / tD);
	//get the difference of the two closest points
	Vector3   dP = w + (sc * u) - (tc * v);// =  S1(sc) - S2(tc)
	return dP.length2();// return the closest distance squared
}
Plane::Plane(const Vector3& normal, float d):normal_(normal), d_(d)
{
	//EMPTY:
}
Plane::Plane(const Vector3& a, const Vector3& b, const Vector3& c)
{
	//Compute vectors from a to b and a to c
	Vector3 ab = b - a;
	Vector3 ac = c - a;
	//Cross product and normalize to get normal
	normal_ = Math::cross(ab, ac);
	normal_.normalize();
	//d = -P dot n
	d_ = -Math::dot(a, normal_);
}
float Plane::signedDist(const Vector3& point) const
{
	return Math::dot(point, normal_) - d_;
}
Sphere::Sphere(const Vector3& center, float radius): center_(center), radius_(radius)
{
	//EMPTY:
}
bool Sphere::contains(const Vector3& point) const
{
	//Get distance squared between center and point
	float distSq = (center_ - point).length2();
	return distSq <= (radius_ * radius_);
}
AABB::AABB(const Vector3& min, const Vector3& max): min_(min), max_(max)
{
	//EMPTY:
}
void AABB::updateMinMax(const Vector3& point)
{
	//Update each component separately
	min_.x = Math::minimum(min_.x, point.x);
	min_.y = Math::minimum(min_.y, point.y);
	min_.z = Math::minimum(min_.z, point.z);
	max_.x = Math::maximum(max_.x, point.x);
	max_.y = Math::maximum(max_.y, point.y);
	max_.z = Math::maximum(max_.z, point.z);
}
void AABB::rotate(const Quaternion& q)
{
	//Construct the 8 points for the corners of the box
	std::array<Vector3, 8> points;
	//Min point is always a corner
	points[0] = min_;
	//Permutations with 2 min and 1 max
	points[1] = Vector3(max_.x, min_.y, min_.z);
	points[2] = Vector3(min_.x, max_.y, min_.z);
	points[3] = Vector3(min_.x, min_.y, max_.z);
	//Permutations with 2 max and 1 min
	points[4] = Vector3(min_.x, max_.y, max_.z);
	points[5] = Vector3(max_.x, min_.y, max_.z);
	points[6] = Vector3(max_.x, max_.y, min_.z);
	//Max point corner
	points[7] = Vector3(max_);
	//Rotate first point
	Vector3 p = Math::transform(points[0], q);
	//Reset min/max to first point rotated
	min_ = p;
	max_ = p;
	//Update min/max based on remaining points, rotated
	for(size_t i = 1; i < points.size(); i++)
	{
		p = Math::transform(points[i], q);
		updateMinMax(p);
	}
}
bool AABB::contains(const Vector3& point) const
{
	// If none of these are true, the point is inside the box
	return !(point.x < min_.x || point.y < min_.y || point.z < min_.z || point.x > max_.x || point.y > max_.y || point.z > max_.z);
}
float AABB::minDistSq(const Vector3& point) const
{
	//Compute differences for each axis
	float dx = Math::maximum(min_.x - point.x, 0.0f);
	dx = Math::maximum(dx, point.x - max_.x);
	float dy = Math::maximum(min_.y - point.y, 0.0f);
	dy = Math::maximum(dy, point.y - max_.y);
	float dz = Math::maximum(min_.z - point.z, 0.0f);
	dz = Math::maximum(dy, point.z - max_.z);
	//Distance squared formula
	return dx * dx + dy * dy + dz * dz;
}
Capsule::Capsule(const Vector3& start, const Vector3& end, float radius): segment_(start, end), radius_(radius)
{
	//EMPTY:
}
Vector3 Capsule::pointOnSegment(float t) const
{
	return segment_.pointOnSegment(t);
}
bool Capsule::contains(const Vector3& point) const
{
	//Get minimal dist. sq. between point and line segment
	float distSq = segment_.minDistSq(point);
	return distSq <= (radius_ * radius_);
}
bool ConvexPolygon::contains(const Vector2& point) const
{
	float sum = 0.0f;
	Vector2 a, b;
	for (size_t i = 0; i < vertices_.size() - 1; i++)
	{
		//From point to first vertex
		a = vertices_[i] - point;
		a.normalize();
		//From point to second vertex
		b = vertices_[i + 1] - point;
		b.normalize();
		//Add angle to sum
		sum += Math::acos(Math::dot(a, b));
	}
	//Have to add angle for last vertex and first vertex
	a = vertices_.back() - point;
	a.normalize();
	b = vertices_.front() - point;
	b.normalize();
	sum += Math::acos(Math::dot(a, b));
	//Return true if approximately 2pi
	return Math::nearZero(sum - Math::TWO_PI);
}
bool intersect(const Sphere& a, const Sphere& b)
{
	float distSq = (a.center_ - b.center_).length2();
	float sumRadii = a.radius_ + b.radius_;
	return distSq <= (sumRadii * sumRadii);
}
bool intersect(const AABB& a, const AABB& b)
{
	// If none of these are true, they must intersect
	return !(a.max_.x < b.min_.x || a.max_.y < b.min_.y || a.max_.z < b.min_.z || b.max_.x < a.min_.x || b.max_.y < a.min_.y || b.max_.z < a.min_.z);
}
bool intersect(const Capsule& a, const Capsule& b)
{
	float distSq = LineSegment::minDistSq(a.segment_, b.segment_);
	float sumRadii = a.radius_ + b.radius_;
	return distSq <= (sumRadii * sumRadii);
}
bool intersect(const Sphere& s, const AABB& box)
{
	float distSq = box.minDistSq(s.center_);
	return distSq <= (s.radius_ * s.radius_);
}
bool intersect(const LineSegment& l, const Sphere& s, float& outT)
{
	//Compute X, Y, a, b, c as per equations
	Vector3 X = l.start_ - s.center_;
	Vector3 Y = l.end_ - l.start_;
	float a = Math::dot(Y, Y);
	float b = 2.0f * Math::dot(X, Y);
	float c = Math::dot(X, X) - s.radius_ * s.radius_;
	//Compute discriminant
	float disc = b * b - 4.0f * a * c;
	if(disc < 0.0f)
	{
		return false;
	}
	else
	{
		disc = Math::sqrt(disc);
		//Compute min and max solutions of t
		float tMin = (-b - disc) / (2.0f * a);
		float tMax = (-b + disc) / (2.0f * a);
		//Check whether either t is within bounds of segment
		if(tMin >= 0.0f && tMin <= 1.0f)
		{
			outT = tMin;
			return true;
		}
		else if(tMax >= 0.0f && tMax <= 1.0f)
		{
			outT = tMax;
			return true;
		}
		else
		{
			return false;
		}
	}
}
bool intersect(const LineSegment& l, const Plane& p, float& outT)
{
	//First test if there's a solution for t
	float denom = Math::dot(l.end_ - l.start_, p.normal_);
	if(Math::nearZero(denom))
	{
		//The only way they intersect is if start is a point on the plane (P dot N) == d
		if(Math::nearZero(Math::dot(l.start_, p.normal_) - p.d_))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		float numer = -Math::dot(l.start_, p.normal_) - p.d_;
		outT = numer / denom;
		//Validate t is within bounds of the line segment
		if(outT >= 0.0f && outT <= 1.0f)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}
bool testSidePlane(float start, float end, float negd, const Vector3& norm, std::vector<std::pair<float, Vector3>>& out)
{
	float denom = end - start;
	if (Math::nearZero(denom))
	{
		return false;
	}
	else
	{
		float numer = -start + negd;
		float t = numer / denom;
		//Test that t is within bounds
		if(t >= 0.0f && t <= 1.0f)
		{
			out.emplace_back(t, norm);
			return true;
		}
		else
		{
			return false;
		}
	}
}
bool intersect(const LineSegment& l, const AABB& b, float& outT, Vector3& outNorm)
{
	//Vector to save all possible t values, and normals for those sides
	std::vector<std::pair<float, Vector3>> tValues;
	//Test the x planes
	testSidePlane(l.start_.x, l.end_.x, b.min_.x, Vector3::NEGATIVE_UNIT_X, tValues);
	testSidePlane(l.start_.x, l.end_.x, b.max_.x, Vector3::UNIT_X, tValues);
	//Test the y planes
	testSidePlane(l.start_.y, l.end_.y, b.min_.y, Vector3::NEGATIVE_UNIT_Y, tValues);
	testSidePlane(l.start_.y, l.end_.y, b.max_.y, Vector3::UNIT_Y, tValues);
	//Test the z planes
	testSidePlane(l.start_.z, l.end_.z, b.min_.z, Vector3::NEGATIVE_UNIT_Z, tValues);
	testSidePlane(l.start_.z, l.end_.z, b.max_.z, Vector3::UNIT_Z, tValues);
	//Sort the t values in ascending order
	std::sort(tValues.begin(), tValues.end(), [](const std::pair<float, Vector3>& a, const std::pair<float, Vector3>& b) {
		return a.first < b.first;
	});
	//Test if the box contains any of these points of intersection
	Vector3 point;
	for(auto& t : tValues)
	{
		point = l.pointOnSegment(t.first);
		if (b.contains(point))
		{
			outT = t.first;
			outNorm = t.second;
			return true;
		}
	}
	//None of the intersections are within bounds of box
	return false;
}
bool sweptSphere(const Sphere& P0, const Sphere& P1, const Sphere& Q0, const Sphere& Q1, float& outT)
{
	//Compute X, Y, a, b, and c
	Vector3 X = P0.center_ - Q0.center_;
	Vector3 Y = P1.center_ - P0.center_ - (Q1.center_ - Q0.center_);
	float a = Math::dot(Y, Y);
	float b = 2.0f * Math::dot(X, Y);
	float sumRadii = P0.radius_ + Q0.radius_;
	float c = Math::dot(X, X) - sumRadii * sumRadii;
	//Solve discriminant
	float disc = b * b - 4.0f * a * c;
	if(disc < 0.0f)
	{
		return false;
	}
	else
	{
		disc = Math::sqrt(disc);
		//We only care about the smaller solution
		outT = (-b - disc) / (2.0f * a);
		if(outT >= 0.0f && outT <= 0.0f)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}