#pragma once

#include <string>
#include <vector>

#include "Kanex.hpp"

struct GameplayPlayAnimAndWaitTaskSpec
{
	std::string animation;

	std::string completed;
	std::string canceled;

	GameplayPlayAnimAndWaitTaskSpec() = default;

	GameplayPlayAnimAndWaitTaskSpec(std::string_view _animation, std::string_view _completed, std::string _canceled)
		: animation(_animation)
		, completed(_completed)
		, canceled(_canceled)
	{

	}
};

NON_MEMEBER_BAR(GameplayPlayAnimAndWaitTaskSpec, obj.animation, obj.completed, obj.canceled);

struct GameplayPlayAnimAndWaitTask
{
	std::vector<GameplayPlayAnimAndWaitTaskSpec> value;
};

NON_MEMEBER_BAR(GameplayPlayAnimAndWaitTask, obj.value);