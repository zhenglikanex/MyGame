#pragma once

#include "Framework/Game/GameHelper.hpp"

class ServerGameHelper : public GameHelper
{
public:
	GameMode GetGameMode() const override;

	void CreateSystems(Game* game) const override;
private:

};
