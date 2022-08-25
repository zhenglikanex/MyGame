#pragma once

#include <string>
#include <string_view>

#include "Framework/Game/Math.hpp"

#include "Kanex.hpp"

struct GameplayAbilityTimelineTask
{
	std::string name;
	std::string completed;
	uint16_t frame;
	std::vector<mat4> collider_event_start_transforms;
	bool is_done;

	GameplayAbilityTimelineTask() = default;
	GameplayAbilityTimelineTask(std::string_view _name,std::string_view _completed)
		: name(_name)
		, completed(_completed)
		, frame(0)
		, is_done(false)
	{
		
	}
};

NON_MEMEBER_BAR(GameplayAbilityTimelineTask, obj.name, obj.completed, obj.frame, obj.collider_event_start_transforms, obj.is_done);