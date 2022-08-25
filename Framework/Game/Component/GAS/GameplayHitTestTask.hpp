#pragma once

#include <vector>
#include <string>

#include "Framework/Game/Component/Collider.hpp"
#include "Framework/Game/Component/Transform.hpp"

enum class GameplayHitType : uint8_t
{
	kRay,
	kOverlap,
	kSweep
};

struct GameplayHitTestSpec
{
	GameplayHitType type;
	Geometry geometry;
	Transform transform;
	std::string on_hit;

	GameplayHitTestSpec() = default;
	GameplayHitTestSpec(GameplayHitType _type,const Geometry& _geometry,const Transform& _transform,std::string_view _on_hit)
		: type(_type)
		, geometry(_geometry)
		, transform(_transform)
		, on_hit(_on_hit)
	{

	}
};

NON_MEMEBER_BAR(GameplayHitTestSpec, obj.type, obj.geometry, obj.transform, obj.on_hit);

struct GameplayHitTestTask
{
	std::vector<GameplayHitTestSpec> value;

	GameplayHitTestTask() = default;
};

NON_MEMEBER_BAR(GameplayHitTestTask, obj.value);