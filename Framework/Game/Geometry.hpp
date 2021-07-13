#pragma once

#include "Framework/Game/Math.hpp"

#include "Kanex.hpp"

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
	float r; // Sphere radius

	Sphere() = default;

	Sphere(float _r)
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
	float h;
	float r;      // Radius

	Capsule() = default;

	Capsule(float _h, float _r)
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

NON_MEMEBER_BAR(Sphere, obj.r);
NON_MEMEBER_BAR(OBB, obj.e);
NON_MEMEBER_BAR(Capsule, obj.h, obj.r);

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

	void Save(kanex::BinaryOutputArchive& ar) const
	{
		ar(type_);
		if (type_ == GeometryType::kSphere)
		{
			ar(sphere_);
		}
		else if (type_ == GeometryType::kBox)
		{
			ar(box_);
		}
		else if (type_ == GeometryType::kCapsule)
		{
			ar(capsule_);
		}
	}

	void Load(kanex::BinaryInputArchive& ar)
	{
		ar(type_);
		if (type_ == GeometryType::kSphere)
		{
			ar(sphere_);
		}
		else if (type_ == GeometryType::kBox)
		{
			ar(box_);
		}
		else if (type_ == GeometryType::kCapsule)
		{
			ar(capsule_);
		}
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