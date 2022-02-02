#pragma once

#include <cstdint>
#include <vector>
#include <memory>

#include "Framework/Game/Service.hpp"

class System;
class Registry;

class GameHelper : public Service
{
public:
	virtual bool IsAuthority() const = 0;
	virtual std::vector<std::unique_ptr<System>> CreateSystems(Registry* registry) const = 0;

	virtual bool IsLocalPlayer(uint32_t id) const = 0;
	virtual bool IsRemotePlayer(uint32_t id) const = 0;
private:
};