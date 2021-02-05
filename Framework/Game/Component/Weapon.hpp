#pragma once

#include "Framework/Game/Component/BoundingBox.hpp"

struct Weapon
{
	BoundingBox bounding_box;
	
	Weapon(const BoundingBox& box)
		:bounding_box(box)
	{

	}
};