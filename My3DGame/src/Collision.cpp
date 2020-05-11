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

LineSegment::LineSegment(const vector3& start, const vector3& end): start_(start), end_(end)
{
	//EMPTY:
}
vector3 LineSegment::pointOnSegment(float t) const
{
	return start_ + (end_ - start_) * t;
}
float LineSegment::minDistSq(const vector3& point) const
{
	//Construct vectors
	vector3 ab = end_ - start_;
	vector3 ba = -1.0f * ab;
	vector3 ac = point - start_;
	vector3 bc = point - end_;
	//Case 1: C projects prior to A
	if(vector3::Dot(ab, ac) < 0.0f)
	{
		return ac.Length2();
	}
	//Case 2: C projects after B
	else if(vector3::Dot(ba, bc) < 0.0f)
	{
		return bc.Length2();
	}
	//Case 3: C projects onto line
	else
	{
		//Compute p
		float scalar = vector3::Dot(ac, ab) / vector3::Dot(ab, ab);
		vector3 p = scalar * ab;
		//Compute length squared of ac - p
		return (ac - p).Length2();
	}
}
float LineSegment::minDistSq(const LineSegment & s1, const LineSegment & s2)
{
	vector3   u = s1.end_ - s1.start_;
	vector3   v = s2.end_ - s2.start_;
	vector3   w = s1.start_ - s2.start_;
	//always >= 0
	float    a = vector3::Dot(u, u);
	float    b = vector3::Dot(u, v);
	//always >= 0
	float    c = vector3::Dot(v, v);
	float    d = vector3::Dot(u, w);
	float    e = vector3::Dot(v, w);
	//always >= 0
	float    D = a * c - b * b;
	//sc = sN / sD, default sD = D >= 0
	float    sc, sN, sD = D;
	//tc = tN / tD, default tD = D >= 0
	float    tc, tN, tD = D;
	//compute the line parameters of the two closest points
	//the lines are almost parallel
	if(Math::NearZero(D))
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
	sc = (Math::NearZero(sN) ? 0.0f : sN / sD);
	tc = (Math::NearZero(tN) ? 0.0f : tN / tD);
	//get the difference of the two closest points
	vector3   dP = w + (sc * u) - (tc * v);// =  S1(sc) - S2(tc)
	return dP.Length2();// return the closest distance squared
}
Plane::Plane(const vector3& normal, float d):normal_(normal), d_(d)
{
	//EMPTY:
}
Plane::Plane(const vector3& a, const vector3& b, const vector3& c)
{
	//Compute vectors from a to b and a to c
	vector3 ab = b - a;
	vector3 ac = c - a;
	//Cross product and normalize to get normal
	normal_ = vector3::Cross(ab, ac);
	normal_.Normalize();
	//d = -P dot n
	d_ = -vector3::Dot(a, normal_);
}
float Plane::signedDist(const vector3& point) const
{
	return vector3::Dot(point, normal_) - d_;
}
Sphere::Sphere(const vector3& center, float radius): center_(center), radius_(radius)
{
	//EMPTY:
}
bool Sphere::contains(const vector3& point) const
{
	// Get distance squared between center and point
	float distSq = (center_ - point).Length2();
	return distSq <= (radius_ * radius_);
}
AABB::AABB(const vector3& min, const vector3& max): min_(min), max_(max)
{
	//EMPTY:
}
void AABB::updateMinMax(const vector3& point)
{
	//Update each component separately
	min_.x = Math::Min(min_.x, point.x);
	min_.y = Math::Min(min_.y, point.y);
	min_.z = Math::Min(min_.z, point.z);
	max_.x = Math::Max(max_.x, point.x);
	max_.y = Math::Max(max_.y, point.y);
	max_.z = Math::Max(max_.z, point.z);
}
void AABB::rotate(const quaternion& q)
{
	//Construct the 8 points for the corners of the box
	std::array<vector3, 8> points;
	//Min point is always a corner
	points[0] = min_;
	//Permutations with 2 min and 1 max
	points[1] = vector3(max_.x, min_.y, min_.z);
	points[2] = vector3(min_.x, max_.y, min_.z);
	points[3] = vector3(min_.x, min_.y, max_.z);
	//Permutations with 2 max and 1 min
	points[4] = vector3(min_.x, max_.y, max_.z);
	points[5] = vector3(max_.x, min_.y, max_.z);
	points[6] = vector3(max_.x, max_.y, min_.z);
	//Max point corner
	points[7] = vector3(max_);
	//Rotate first point
	vector3 p = vector3::Transform(points[0], q);
	//Reset min/max to first point rotated
	min_ = p;
	max_ = p;
	//Update min/max based on remaining points, rotated
	for(size_t i = 1; i < points.size(); i++)
	{
		p = vector3::Transform(points[i], q);
		updateMinMax(p);
	}
}
bool AABB::contains(const vector3& point) const
{
	// If none of these are true, the point is inside the box
	return !(point.x < min_.x || point.y < min_.y || point.z < min_.z || point.x > max_.x || point.y > max_.y || point.z > max_.z);
}
float AABB::minDistSq(const vector3& point) const
{
	//Compute differences for each axis
	float dx = Math::Max(min_.x - point.x, 0.0f);
	dx = Math::Max(dx, point.x - max_.x);
	float dy = Math::Max(min_.y - point.y, 0.0f);
	dy = Math::Max(dy, point.y - max_.y);
	float dz = Math::Max(min_.z - point.z, 0.0f);
	dz = Math::Max(dy, point.z - max_.z);
	//Distance squared formula
	return dx * dx + dy * dy + dz * dz;
}
Capsule::Capsule(const vector3& start, const vector3& end, float radius): segment_(start, end), radius_(radius)
{
	//EMPTY:
}
vector3 Capsule::pointOnSegment(float t) const
{
	return segment_.pointOnSegment(t);
}
bool Capsule::contains(const vector3& point) const
{
	//Get minimal dist. sq. between point and line segment
	float distSq = segment_.minDistSq(point);
	return distSq <= (radius_ * radius_);
}
bool ConvexPolygon::contains(const vector2& point) const
{
	float sum = 0.0f;
	vector2 a, b;
	for (size_t i = 0; i < vertices_.size() - 1; i++)
	{
		//From point to first vertex
		a = vertices_[i] - point;
		a.Normalize();
		//From point to second vertex
		b = vertices_[i + 1] - point;
		b.Normalize();
		//Add angle to sum
		sum += Math::Acos(vector2::Dot(a, b));
	}
	//Have to add angle for last vertex and first vertex
	a = vertices_.back() - point;
	a.Normalize();
	b = vertices_.front() - point;
	b.Normalize();
	sum += Math::Acos(vector2::Dot(a, b));
	//Return true if approximately 2pi
	return Math::NearZero(sum - Math::TwoPi);
}
bool intersect(const Sphere& a, const Sphere& b)
{
	float distSq = (a.center_ - b.center_).Length2();
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
	vector3 X = l.start_ - s.center_;
	vector3 Y = l.end_ - l.start_;
	float a = vector3::Dot(Y, Y);
	float b = 2.0f * vector3::Dot(X, Y);
	float c = vector3::Dot(X, X) - s.radius_ * s.radius_;
	//Compute discriminant
	float disc = b * b - 4.0f * a * c;
	if(disc < 0.0f)
	{
		return false;
	}
	else
	{
		disc = Math::Sqrt(disc);
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
	float denom = vector3::Dot(l.end_ - l.start_, p.normal_);
	if(Math::NearZero(denom))
	{
		//The only way they intersect is if start is a point on the plane (P dot N) == d
		if(Math::NearZero(vector3::Dot(l.start_, p.normal_) - p.d_))
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
		float numer = -vector3::Dot(l.start_, p.normal_) - p.d_;
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
bool testSidePlane(float start, float end, float negd, const vector3& norm, std::vector<std::pair<float, vector3>>& out)
{
	float denom = end - start;
	if (Math::NearZero(denom))
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
bool intersect(const LineSegment& l, const AABB& b, float& outT, vector3& outNorm)
{
	//Vector to save all possible t values, and normals for those sides
	std::vector<std::pair<float, vector3>> tValues;
	//Test the x planes
	testSidePlane(l.start_.x, l.end_.x, b.min_.x, vector3::NegUnitX, tValues);
	testSidePlane(l.start_.x, l.end_.x, b.max_.x, vector3::UnitX, tValues);
	//Test the y planes
	testSidePlane(l.start_.y, l.end_.y, b.min_.y, vector3::NegUnitY, tValues);
	testSidePlane(l.start_.y, l.end_.y, b.max_.y, vector3::UnitY, tValues);
	//Test the z planes
	testSidePlane(l.start_.z, l.end_.z, b.min_.z, vector3::NegUnitZ, tValues);
	testSidePlane(l.start_.z, l.end_.z, b.max_.z, vector3::UnitZ, tValues);
	//Sort the t values in ascending order
	std::sort(tValues.begin(), tValues.end(), [](const std::pair<float, vector3>& a, const std::pair<float, vector3>& b) {
		return a.first < b.first;
	});
	//Test if the box contains any of these points of intersection
	vector3 point;
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
	vector3 X = P0.center_ - Q0.center_;
	vector3 Y = P1.center_ - P0.center_ - (Q1.center_ - Q0.center_);
	float a = vector3::Dot(Y, Y);
	float b = 2.0f * vector3::Dot(X, Y);
	float sumRadii = P0.radius_ + Q0.radius_;
	float c = vector3::Dot(X, X) - sumRadii * sumRadii;
	//Solve discriminant
	float disc = b * b - 4.0f * a * c;
	if(disc < 0.0f)
	{
		return false;
	}
	else
	{
		disc = Math::Sqrt(disc);
		//We only care about the smaller solution
		outT = (-b - disc) / (2.0f * a);
		if (outT >= 0.0f && outT <= 0.0f)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}