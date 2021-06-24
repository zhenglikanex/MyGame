#pragma once

#include "Framework/Game/GameConfig.hpp"

#include "Framework/Game/Component/Transform.hpp"


struct TransformState
{
	std::array<Transform, GameConfig::kMaxPredictFrame> value;

	TransformState() = default;
};