#pragma once

#include <string>

#include "Framework/Game/FixedPoint.hpp"

#include "Framework/serialize/binary_archive.hpp"

struct AnimationClip
{
	std::string name;
	fixed16 time;
	bool is_done;
	AnimationClip(std::string _name)
		: name(_name)
		, time(0)
		, is_done(false)
	{
	}
};

