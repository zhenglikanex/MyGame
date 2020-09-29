#pragma once

#include "Framework/Game/Math.hpp"
#include "Framework/serialize/binary_archive.hpp"

struct Movement
{
	fixed16 velocity;

	BAR(velocity);
};