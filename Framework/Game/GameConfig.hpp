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

	const float kFrameTime = 0.033f;
	const float kDefaultInputFrame = 0.033f;
	constexpr uint32_t kMaxPredictFrame = 2000;
	const uint32_t kMaxOnceFrameRunNum = 100;
	const float kMaxRollbackFrameNum = 2000; //backwards reconcilation最大向后缓和(延迟补偿)时间
}

enum class GameMode
{
	kClinet,
	kServer
};
