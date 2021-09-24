#pragma once

#include "Framework/Game/Geometry.hpp"

#include "3rdparty/include/entt/entt.hpp"

enum class RigidType : uint8_t
{
	kStatic,
	kDynamic,
	kKinematic
};

struct ColliderInfo
{
	Geometry geometry;
	mat4 transform;
	bool trigger;
	RigidType rigid_type;

	ColliderInfo() = default;

	ColliderInfo(const Geometry& _geometry, const mat4& _transform, bool _trigger, RigidType _rigid_type)
		: geometry(_geometry)
		, transform(_transform)
		, trigger(_trigger)
		, rigid_type(_rigid_type)
	{

	}
};

inline void from_json(const nlohmann::json& j, ColliderInfo& info)
{
	std::string type = j["type"];

	assert((type == "sphere" || type == "box" || type == "capsule") && "not support collision type");

	if (type == "sphere")
	{
		float r;
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
		float h;
		j.at("half_h").get_to(h);

		float r;
		j.at("r").get_to(r);

		info.geometry = Geometry(Capsule(h, r));
	}

	j.at("transform").get_to(info.transform);
	j.at("trigger").get_to(info.trigger);
	j.at("rigid_type").get_to(info.rigid_type);
}

NON_MEMEBER_BAR(ColliderInfo, obj.geometry, obj.transform, obj.trigger);