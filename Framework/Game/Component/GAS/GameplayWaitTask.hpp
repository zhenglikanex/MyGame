#pragma once

#include <string_view>
#include <vector>

#include "Kanex.hpp"

struct GameplayWaitTaskSpec
{
	float time;
	std::string completed;

	GameplayWaitTaskSpec() = default;

	GameplayWaitTaskSpec(float _time, std::string_view _completed)
		: time(_time)
		, completed(_completed)
	{

	}
};

NON_MEMEBER_BAR(GameplayWaitTaskSpec, obj.time, obj.completed);

struct GameplayWaitTask
{
	std::vector<GameplayWaitTaskSpec> value;

	GameplayWaitTask() = default;
};

NON_MEMEBER_BAR(GameplayWaitTask, obj.value);