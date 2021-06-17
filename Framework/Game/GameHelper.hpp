#pragma once

#include <cstdint>

class GameHelper
{
public:
	GameHelper() = default;
	GameHelper(const GameHelper&) = delete;
	const GameHelper& operator=(const GameHelper&) = delete;

	virtual ~GameHelper() = 0 {}

	virtual bool IsLocalPlayer(uint32_t id) const = 0;
	virtual bool IsRemotePlayer(uint32_t id) const = 0;
private:
};