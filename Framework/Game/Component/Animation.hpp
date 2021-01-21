#pragma once

#include <string>

#include "Framework/Game/FixedPoint.hpp"

#include "Framework/serialize/binary_archive.hpp"

struct Animation
{
	std::string name;
	uint32_t max_frame;
	fixed16 time;
	bool is_done;
	
	Animation(std::string _name)
		: name(_name)
		, max_frame(0)
		, time(0)
		, is_done(false)
	{
		// todo ����
		max_frame = 0;
	}
};
