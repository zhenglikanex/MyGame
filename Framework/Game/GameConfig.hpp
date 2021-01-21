#pragma once

#include <cstdint>
#include <string>

#include "Framework/Game/FixedPoint.hpp"

namespace GameConfig
{
	namespace ActionAnimation
	{
		const std::string kIdle = "idle";
		const std::string kMovement = "movement";
		const std::string kJump = "jump";
		const std::string kDeath = "death";
	}

	const fixed16 kFrameTime = fixed16(0.33);
}