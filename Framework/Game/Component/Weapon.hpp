#pragma once

#include "Framework/Game/Component/ColliderInfo.hpp"

#include "Kanex.hpp"

struct Weapon
{
	ColliderInfo collider_info;

	Weapon() = default;

	Weapon(const ColliderInfo& _collider_info)
		:collider_info(_collider_info)
	{

	}
};

NON_MEMEBER_BAR(Weapon, obj.collider_info);