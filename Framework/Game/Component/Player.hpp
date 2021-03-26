#pragma once

#include <cstdint>

#include "Kanex.hpp"

struct Player
{
	uint32_t id;

	Player() = default;
	Player(uint32_t _id)
		: id(_id)
	{

	}
};

NON_MEMEBER_BAR(Player, obj.id);

