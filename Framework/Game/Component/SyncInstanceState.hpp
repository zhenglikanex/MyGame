#pragma once

#include "Framework/Game/GameConfig.hpp"

#include "Framework/Game/Component/Local.hpp"

struct SyncInstanceState
{
	std::array<uint32_t,GameConfig::kMaxPredictFrame> value;
};