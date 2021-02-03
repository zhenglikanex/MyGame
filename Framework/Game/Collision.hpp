#pragma once

#include "glm/glm.hpp"
#include "Framework/Game/Math.hpp"

class Primitive
{

};

struct Sphere {
	vec3 c; // Sphere center
	fixed16 r; // Sphere radius
};

struct AABB {
	vec3 c; // center point of AABB
	vec3 r; // radius or halfwidth extents (rx, ry, rz)
	AABB(const vec3& _c,const vec3& _r)
		:c(_c)
		,r(_r)
	{ }
};

struct OBB {
	vec3 c;     // OBB center point
	vec3 u[3]; // Local x-, y-, and z-axes
	vec3 e;    // Positive halfwidth extents of OBB along each axis
};

struct Capsule {
	vec3 a;      // Medial line segment start point
	vec3 b;      // Medial line segment end point
	fixed16 r;      // Radius
};

struct Ray
{
	vec3 p;
	vec3 d;
};