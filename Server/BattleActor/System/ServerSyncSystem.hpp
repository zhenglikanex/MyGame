#pragma once

#include "Framework/Game/System.hpp"

#include "Framework/Game/Component/Transform.hpp"
#include "Framework/Game/Component/Player.hpp"

struct ServerSyncSystem : public System
{
	ServerSyncSystem(entt::registry& _registry)
		: System(_registry)
	{
		
	}

	~ServerSyncSystem()
	{

	}

	bool Initialize() override
	{

	}


	void FixedUpdate(fixed16 dt) override
	{
		
	}

	void Finalize() override
	{

	}

	void SyncPlayer()
	{
		auto view = registry.view<Player, Transform>();
		for (auto e : view)
		{
			auto& [player, transfrom] = view.get<Player, Transform>(e);
		}
	}
};