#pragma once

#include "Framework/Game/Component/BoundingBox.hpp"
#include "Framework/Game/Component/Health.hpp"

struct Weapon
{
	BoundingBox bounding_box;
	std::vector<Attribute> attributes;

	Weapon(const BoundingBox& box)
		:bounding_box(box)
	{

	}
};
