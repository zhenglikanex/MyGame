#pragma once

#include "Framework/Game/Math.hpp"

struct AABB 
{
	vec3 r; // radius or halfwidth extents (rx, ry, rz)
	AABB(const vec3& _r)
		: r(_r)
	{
	}
};

struct Sphere 
{
	fixed16 r; // Sphere radius

	Sphere(fixed16 _r)
		: r(_r)
	{
	}

	AABB GetAABB() const
	{
		return AABB(vec3(r, r, r));
	}
};

struct OBB 
{
	vec3 e;    // Positive halfwidth extents of OBB along each axis

	OBB(const vec3& _e)
		: e(_e)
	{
	}

	AABB GetAABB() const
	{
		return AABB(vec3(e, e, e));
	}
};

struct Capsule
{
	fixed16 h;
	fixed16 r;      // Radius

	Capsule(fixed16 _r)
		: r(_r)
	{
	}

	AABB GetAABB() const
	{
		return AABB(vec3(r, r, r));
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

enum class GeometryType : uint8_t
{
	kSphere,
	kBox,
	kCapsule,
	kMax
};

class Geometry
{
public:
	union 
	{
		Sphere sphere;
		OBB box;
		Capsule capsule;
	};

	Geometry(const Sphere& _sphere)
		: type_(GeometryType::kSphere)
		, sphere(_sphere)
	{

	}

	Geometry(const OBB& _obb)
		: type_(GeometryType::kBox)
		, box(_obb)
	{

	}

	Geometry(const Capsule& _capsule)
		: type_(GeometryType::kCapsule)
		, capsule(_capsule)
	{

	}

	AABB GetAABB() const
	{
		if (type_ == GeometryType::kSphere)
		{
			return sphere.GetAABB();
		}
		else if (type_ == GeometryType::kBox)
		{
			return box.GetAABB();
		}
		else if (type_ == GeometryType::kCapsule)
		{
			return capsule.GetAABB();
		}

		assert(false);
	}

	GeometryType type() const { return type_; }

private:
	GeometryType type_;
};
