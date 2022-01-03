#pragma once

#include <string>
#include <vector>

#include "Kanex.hpp"

struct GameplayPlayAnimAndWaitTaskSpec
{
	std::string animation;
	std::string completed;
	std::string canceled;
	bool running;

	GameplayPlayAnimAndWaitTaskSpec() = default;

	GameplayPlayAnimAndWaitTaskSpec(std::string_view _animation, std::string_view _completed, std::string_view _canceled)
		: animation(_animation)
		, completed(_completed)
		, canceled(_canceled)
		, running(false)
	{

	}
};

NON_MEMEBER_BAR(GameplayPlayAnimAndWaitTaskSpec, obj.animation, obj.completed, obj.canceled);

struct GameplayPlayAnimAndWaitTask
{
	std::vector<GameplayPlayAnimAndWaitTaskSpec> value;

	GameplayPlayAnimAndWaitTask() = default;
};

NON_MEMEBER_BAR(GameplayPlayAnimAndWaitTask, obj.value);