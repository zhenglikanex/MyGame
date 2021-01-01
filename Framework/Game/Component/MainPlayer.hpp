#pragma once

#include <cstdint>
#include "Framework/serialize/binary_archive.hpp"

struct MainPlayer
{
	uint32_t value;

	BAR(value);
};