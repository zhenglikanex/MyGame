#pragma once

#include "Framework/Game/GameConfig.hpp"
#include "Framework/Game/Component/SyncManager.hpp"

struct SyncInstanceState
{
	std::array<SyncInstance,GameConfig::kMaxPredictFrame> value;
};