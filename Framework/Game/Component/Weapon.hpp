#pragma once

#include "Framework/Game/Component/BoundingBox.hpp"
#include "Framework/Game/Component/Health.hpp"

struct Weapon
{
	Collider bounding_box;
	std::vector<Attribute> attributes;

	Weapon(const Collider& box)
		:bounding_box(box)
	{

	}
};
