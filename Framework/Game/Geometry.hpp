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

	Sphere() = default;

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

	OBB() = default;

	OBB(const vec3& _e)
		: e(_e)
	{
	}

	AABB GetAABB() const
	{
		return AABB(e);
	}
};

struct Capsule
{
	fixed16 h;
	fixed16 r;      // Radius

	Capsule() = default;

	Capsule(fixed16 _h, fixed16 _r)
		:h(_h), r(_r)
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

	Ray() = default;

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
	

	Geometry() = default;

	explicit Geometry(const Sphere& _sphere)
		: type_(GeometryType::kSphere)
		, sphere_(_sphere)
	{

	}

	explicit Geometry(const OBB& _obb)
		: type_(GeometryType::kBox)
		, box_(_obb)
	{

	}

	explicit Geometry(const Capsule& _capsule)
		: type_(GeometryType::kCapsule)
		, capsule_(_capsule)
	{

	}

	AABB GetAABB() const
	{
		if (type_ == GeometryType::kSphere)
		{
			return sphere_.GetAABB();
		}
		else if (type_ == GeometryType::kBox)
		{
			return box_.GetAABB();
		}
		else if (type_ == GeometryType::kCapsule)
		{
			return capsule_.GetAABB();
		}

		assert(false);
	}

	GeometryType type() const { return type_; }

	const Sphere& sphere() const
	{
		assert(type_ == GeometryType::kSphere && "type is not sphere");

		return sphere_;
	}

	const OBB& box() const
	{
		assert(type_ == GeometryType::kBox && "type is not box");

		return box_;
	}

	const Capsule& capsule() const
	{
		assert(type_ == GeometryType::kCapsule && "type is not capsule");

		return capsule_;
	}

private:
	union
	{
		Sphere sphere_;
		OBB box_;
		Capsule capsule_;
	};

	GeometryType type_;
};
