#pragma once

#include <cstdint>
#include "Framework/serialize/binary_archive.hpp"

struct Player
{
	uint32_t id;
	uint32_t actor_id;

	BAR(id,actor_id);
};