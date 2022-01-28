#pragma once

#include "Framework/Game/Component/Collider.hpp"

#include "Kanex.hpp"

struct Weapon
{
	Collider collider;

	Weapon() = default;

	Weapon(const Collider& _collider)
		:collider(_collider)
	{

	}
};

NON_MEMEBER_BAR(Weapon, obj.collider);