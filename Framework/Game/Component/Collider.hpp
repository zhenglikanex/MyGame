#pragma once

#include "Framework/Game/Collision.hpp"
#include "Framework/Game/Json.hpp"

struct Collider
{
	Geometry geometry;
	bool trigger;

	Collider(const Geometry& _gemoetry, bool _trigger)
		:geometry(_gemoetry)
		, trigger(_trigger)
	{

	}
};

inline Collider GetCollider(std::string_view str)
{
	nlohmann::json json = nlohmann::json::parse(str);
	return GetCollider(json);
}

inline Collider GetCollider(const nlohmann::json& json)
{
	std::string type = json["type"];

	assert((type == "sphere" || type == "aabb" || type == "obb" || type == "capsule") && "not support collision type");

	if (type == "sphere")
	{
		vec3 c = zero<vec3>();
		json.at("c").get_to(c);

		fixed16 r = fixed16(0);
		json.at("r").get_to(r);

		Collider collider(Geometry(Sphere(c, r)),true);
		return collider;
	}
	else if (type == "aabb")
	{
		vec3 c = zero<vec3>();
		json.at("c").get_to(c);

		vec3 r = zero<vec3>();
		json.at("r").get_to(r);

		Collider collider(Geometry(AABB(c,r)),true);
		return collider;
	}
	else if (type == "obb")
	{
		vec3 c = zero<vec3>();
		json.at("c").get_to(c);
		
		mat3 u = zero<mat3>();
		json.at("u").get_to(u);

		vec3 e = zero<vec3>();
		json.at("e").get_to(e);

		Collider collider(Geometry(OBB(c,u[0], u[1], u[2],e)),true);
		return collider;
	}
	else
	{
		vec3 a = zero<vec3>();
		json.at("a").get_to(a);
		
		vec3 b = zero<vec3>();
		json.at("b").get_to(b);

		fixed16 r = fixed16(0);
		json.at("r").get_to(r);

		Collider collider(Geometry(Capsule(a,b,r)),true);
		return collider;
	}
}