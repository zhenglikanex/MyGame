#pragma once

#include <string>
#include <vector>

#include "Kanex.hpp"

struct GameplayAnimWaitTask
{
	std::string animation;
	std::string completed;
	std::string canceled;
	bool running;

	GameplayAnimWaitTask() = default;

	GameplayAnimWaitTask(std::string_view _completed, std::string_view _canceled)
		: completed(_completed)
		, canceled(_canceled)
		, running(false)
	{

	}
};

NON_MEMEBER_BAR(GameplayAnimWaitTask, obj.completed, obj.canceled);
