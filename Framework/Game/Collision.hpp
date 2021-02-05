#pragma once

#include "Framework/Game/Math.hpp"

struct Sphere {
	vec3 c; // Sphere center
	fixed16 r; // Sphere radius

	Sphere(const vec3& _c, fixed16 _r)
		: c(_c)
		, r(_r)
	{
	}
};

struct AABB {
	vec3 c; // center point of AABB
	vec3 r; // radius or halfwidth extents (rx, ry, rz)
	AABB(const vec3& _c,const vec3& _r)
		: c(_c)
		, r(_r)
	{
	}
};

struct OBB {
	vec3 c;     // OBB center point
	vec3 u[3]; // Local x-, y-, and z-axes
	vec3 e;    // Positive halfwidth extents of OBB along each axis

	OBB(const vec3& _c,const vec3& x_axis,const vec3& y_axis,const vec3& z_axis,const vec3& _e)
		: c(_c) 
		, u{x_axis,y_axis,z_axis}
		, e(e)
	{
	}
};

struct Capsule {
	vec3 a;      // Medial line segment start point
	vec3 b;      // Medial line segment end point
	fixed16 r;      // Radius

	Capsule(const vec3& _a, const vec3& _b, fixed16 _r)
		: a(_a)
		, b(_b)
		, r(_r)
	{
	}
};

struct Ray
{
	vec3 p;
	vec3 d;

	Ray(const vec3& _p, const vec3& _d)
		: p(_p)
		, d(_d)
	{
	}
};