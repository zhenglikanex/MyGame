#pragma once

#include <string>

#include "Framework/Game/FixedPoint.hpp"

struct AnimationClip
{
	std::string name;
	fixed16 time;
	bool is_done;
	
	AnimationClip() = default;

	AnimationClip(std::string_view _name)
		: name(_name)
		, time(0)
		, is_done(false)
	{

	}

	AnimationClip(std::string_view _name, fixed16 _time)
		: name(_name)
		, time(_time)
		, is_done(false)
	{

	}
};

NON_MEMEBER_BAR(AnimationClip, obj.name, obj.time, obj.is_done);