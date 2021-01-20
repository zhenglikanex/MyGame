#pragma once

#include "Framework/Game/System.hpp"
#include "Framework/Game/Locator.hpp"
#include "Framework/Game/NetworkService.hpp"
#include "Framework/Game/Component/Player.hpp"

struct SyncSystem : public System
{
	SyncSystem(entt::registry& registry)
		:System(registry)
	{

	}

	virtual ~SyncSystem()
	{

	}

	bool Initialize() override
	{
		return true;
	}

	void Update(fixed16 dt) override
	{
		auto view = registry.view<Player>();
		// todo;
		//registry.ctx<Locator>().Get<NetworkService>()->Send(nullptr, 0);
	}

	void Finalize() override
	{

	}
};