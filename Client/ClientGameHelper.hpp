#pragma once

#include "Framework/Game/GameMode.hpp"
#include "Framework/Game/GameHelper.hpp"

class ClientGameHelper : public GameHelper
{
public:
	ClientGameHelper(uint32_t local_id);
	~ClientGameHelper();
	
	GameMode GetGameMode() const override;

	void CreateSystems(Game* game) const override;
	
	uint64_t GameFrameTime() const override;
private:
	uint32_t local_id_;
};