#pragma once

#include <string>
#include <unordered_map>

#include "Framework/Game/FixedPoint.hpp"

struct AnimationClip
{
	std::string name;
	float time;
	bool is_done;
	bool is_timeline;
	
	AnimationClip() = default;

	AnimationClip(std::string_view _name,bool _is_timeline = false)
		: name(_name)
		, time(0)
		, is_done(false)
		, is_timeline(_is_timeline)
	{

	}

	

	AnimationClip(std::string_view _name, float _time,bool _is_timeline = false)
		: name(_name)
		, time(_time)
		, is_done(false)
		, is_timeline(_is_timeline)
	{

	}
};

NON_MEMEBER_BAR(AnimationClip, obj.name, obj.time, obj.is_done,obj.is_timeline);