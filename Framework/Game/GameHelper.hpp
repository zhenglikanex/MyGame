#pragma once

#include <cstdint>

#include "Framework/Game/GameMode.hpp"
#include "Framework/Game/GameConfig.hpp"

class Game;

class GameHelper
{
public:
	GameHelper() = default;
	GameHelper(const GameHelper&) = delete;
	GameHelper(GameHelper&&) = default;
	GameHelper& operator=(const GameHelper&) = delete;
	GameHelper& operator=(GameHelper&&) = default;

	virtual ~GameHelper() = default;

	virtual GameMode GetGameMode() const = 0;

	virtual void CreateSystems(Game* game) const = 0;

	virtual uint64_t GameFrameTime() const 
	{
		return GameConfig::kFrameTimeNanoseconds;
	}
};