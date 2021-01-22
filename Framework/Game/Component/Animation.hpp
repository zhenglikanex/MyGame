#pragma once

#include <string>

#include "Framework/Game/FixedPoint.hpp"

#include "Framework/serialize/binary_archive.hpp"

struct Animation
{
	std::string name;
	fixed16 time;
	bool is_done;
	Animation(std::string _name)
		: name(_name)
		, time(0)
		, is_done(false)
	{
	}
};

