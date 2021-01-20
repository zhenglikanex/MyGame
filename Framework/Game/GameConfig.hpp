#pragma once

#include <cstdint>
#include <string>

#include "Framework/Game/FixedPoint.hpp"

namespace GameConfig
{
	namespace Animation
	{
		const std::string idle = "idle";
		const std::string movement = "movement";
		const std::string jump = "jump";
		const std::string death = "death";
	}

	const fixed16 kFrameTime = fixed16(0.33);
}