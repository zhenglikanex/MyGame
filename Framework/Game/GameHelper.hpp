#pragma once

#include <cstdint>

#include "Framework/Game/System.hpp"
#include "Framework/Game/Service.hpp"

#include "3rdparty/include/entt/entt.hpp"

class GameHelper : public Service
{
public:
	virtual bool IsAuthority() const = 0;
	virtual std::vector<std::unique_ptr<System>> CreateSystems(entt::registry& registry) const = 0;

	virtual bool IsLocalPlayer(uint32_t id) const = 0;
	virtual bool IsRemotePlayer(uint32_t id) const = 0;
	virtual bool IsControllable(entt::registry& registry, entt::entity e) const = 0;
private:
};