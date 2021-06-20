#pragma once

#include <cstdint>

#include "Framework/Game/System.hpp"

#include "3rdparty/include/entt/entt.hpp"

class GameHelper
{
public:
	GameHelper() = default;
	GameHelper(const GameHelper&) = delete;
	const GameHelper& operator=(const GameHelper&) = delete;

	virtual ~GameHelper() = 0 {}

	virtual std::vector<std::unique_ptr<System>> CreateSystems(entt::registry& registry) const = 0;

	virtual bool IsLocalPlayer(uint32_t id) const = 0;
	virtual bool IsRemotePlayer(uint32_t id) const = 0;
private:
};