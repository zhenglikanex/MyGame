#pragma once

#include <cstdint>
#include <vector>
#include <memory>

#include "Framework/Game/GameMode.hpp"
#include "Framework/Game/GameConfig.hpp"

class System;
class Game;

class GameHelper
{
public:
	virtual GameMode GetGameMode() const = 0;

	virtual void CreateSystems(Game* game) const = 0;

	virtual uint64_t GameFrameTime() const 
	{
		return GameConfig::kFrameTimeNanoseconds;
	}
};