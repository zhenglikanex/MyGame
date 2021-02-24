#pragma once

#include <cstdint>
#include <string>

#include "Framework/Game/FixedPoint.hpp"

namespace GameConfig
{
	namespace ActionAnimation
	{
		const std::string kIdle = "Idle";
		const std::string kLocomotion = "Locomotion";
		const std::string kJump = "Jump";
		const std::string kHurt = "Hurt";
		const std::string kDeath = "Death";
	}

	const fixed16 kFrameTime = fixed16(0.03333334f);
}