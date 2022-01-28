#pragma once

#include "Framework/Game/Component/Collider.hpp"

#include "Kanex.hpp"

struct Weapon
{
	Collider collider_info;

	Weapon() = default;

	Weapon(const Collider& _collider_info)
		:collider_info(_collider_info)
	{

	}
};

NON_MEMEBER_BAR(Weapon, obj.collider_info);