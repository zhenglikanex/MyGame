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
	const float kDefaultInputFrame = 0.03333334f;
	constexpr uint32_t kMaxPredictFrame = 300;
	const uint32_t kMaxOnceFrameRunNum = 2;
}