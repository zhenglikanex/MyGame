#pragma once

#include "Framework/Game/GameHelper.hpp"

class ServerGameHelper : public GameHelper
{
public:
	~ServerGameHelper();

	bool IsLocalPlayer(uint32_t id) const override;
	bool IsRemotePlayer(uint32_t id) const override;
private:
};