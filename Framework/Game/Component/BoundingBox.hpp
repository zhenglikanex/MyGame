#pragma once

#include "Framework/Game/Collision.hpp"
#include "Framework/Game/Json.hpp"

enum class BoundingBoxType
{
	kSphere,
	kAABB,
	kOBB,
	kCapsule,
};


struct BoundingBox
{
	BoundingBoxType type;
	union
	{
		Sphere sphere;
		AABB aabb;
		OBB obb;
		Capsule capsule;
	};

	BoundingBox(const Sphere& _sphere)
		: type(BoundingBoxType::kSphere)
		, sphere(_sphere)
	{

	}

	BoundingBox(const AABB& _aabb)
		: type(BoundingBoxType::kAABB)
		, aabb(_aabb)
	{

	}

	BoundingBox(const OBB& _obb)
		: type(BoundingBoxType::kOBB)
		, obb(_obb)
	{

	}

	BoundingBox(const Capsule& _capsule)
		: type(BoundingBoxType::kCapsule)
		, capsule(_capsule)
	{

	}
};

inline BoundingBox GetBoundingBox(std::string_view str)
{
	nlohmann::json json = nlohmann::json::parse(str);
	return GetBoundingBox(json);
}

inline BoundingBox GetBoundingBox(const nlohmann::json& json)
{
	std::string type = json["type"];

	assert((type == "sphere" || type == "aabb" || type == "obb" || type == "capsule") && "not support collision type");

	if (type == "sphere")
	{
		vec3 c = zero<vec3>();
		json.at("c").get_to(c);

		fixed16 r = fixed16(0);
		json.at("r").get_to(r);

		BoundingBox box(Sphere(c, r));
		return box;
	}
	else if (type == "aabb")
	{
		vec3 c = zero<vec3>();
		json.at("c").get_to(c);

		vec3 r = zero<vec3>();
		json.at("r").get_to(r);

		BoundingBox box(AABB(c,r));
		return box;
	}
	else if (type == "obb")
	{
		vec3 c = zero<vec3>();
		json.at("c").get_to(c);
		
		mat3 u = zero<mat3>();
		json.at("u").get_to(u);

		vec3 e = zero<vec3>();
		json.at("e").get_to(e);

		BoundingBox box(OBB(c,u[0], u[1], u[2],e));
		return box;
	}
	else
	{
		vec3 a = zero<vec3>();
		json.at("a").get_to(a);
		
		vec3 b = zero<vec3>();
		json.at("b").get_to(b);

		fixed16 r = fixed16(0);
		json.at("r").get_to(r);

		BoundingBox box(Capsule(a,b,r));
		return box;
	}
}