#pragma once

#include "Framework/Game/Geometry.hpp"

#include "3rdparty/include/entt/entt.hpp"

struct ColliderInfo
{
	Geometry geometry;
	mat4 transform;
	bool trigger;

	entt::entity collider;

	ColliderInfo()
		:collider(entt::null)
	{

	}
};

inline void from_json(const nlohmann::json& j, ColliderInfo& info)
{
	std::string type = j["type"];

	assert((type == "sphere" || type == "box" || type == "capsule") && "not support collision type");

	if (type == "sphere")
	{
		fixed16 r;
		j.at("r").get_to(r);

		info.geometry = Geometry(Sphere(r));
	}
	else if (type == "box")
	{
		vec3 e;
		j.at("e").get_to(e);

		info.geometry = Geometry(OBB(e));
	}
	else
	{
		fixed16 h;
		j.at("h").get_to(h);

		fixed16 r;
		j.at("r").get_to(r);

		info.geometry = Geometry(Capsule(h, r));
	}

	j.at("transform").get_to(info.transform);
	j.at("trigger").get_to(info.trigger);
}
