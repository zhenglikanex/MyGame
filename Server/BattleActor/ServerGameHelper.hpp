#pragma once

#include "Framework/Game/GameHelper.hpp"


class ServerGameHelper : public GameHelper
{
public:
	~ServerGameHelper();

	bool IsAuthority() const override;

	std::vector<std::unique_ptr<System>> CreateSystems(entt::registry& registry) const override;

	bool IsLocalPlayer(uint32_t id) const override;
	bool IsRemotePlayer(uint32_t id) const override;
	
	bool IsControllable(entt::registry& registry, entt::entity e) const override;
private:
};