#pragma once

#include "Framework/Game/GameHelper.hpp"


class ServerGameHelper : public GameHelper
{
public:
	~ServerGameHelper();

	bool IsClient() const override;
	bool IsServer() const override;

	std::vector<std::unique_ptr<System>> CreateSystems(entt::registry& registry) const override;

	bool IsLocalPlayer(uint32_t id) const override;
	bool IsRemotePlayer(uint32_t id) const override;
private:
};