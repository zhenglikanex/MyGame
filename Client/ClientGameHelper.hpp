#pragma once

#include "Framework/Game/GameHelper.hpp"

class ClientGameHelper : public GameHelper
{
public:
	ClientGameHelper(uint32_t local_id);
	~ClientGameHelper();
	
	bool IsAuthority() const override;

	std::vector<std::unique_ptr<System>> CreateSystems(Registry* registry) const override;

	bool IsLocalPlayer(uint32_t id) const override;
	bool IsRemotePlayer(uint32_t id) const override;
private:
	uint32_t local_id_;
};