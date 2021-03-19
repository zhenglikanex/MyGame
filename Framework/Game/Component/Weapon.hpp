#pragma once

#include "Framework/Game/Component/ColliderInfo.hpp"
#include "Framework/Game/Component/Attributes.hpp"

struct Weapon
{
	ColliderInfo collider_info;
	std::vector<Attribute> attributes;

	Weapon(const ColliderInfo& _collider_info)
		:collider_info(_collider_info)
	{

	}
};
