#pragma once

#include "Framework/Game/System.hpp"
#include "Framework/Game/Locator.hpp"
#include "Framework/Game/GameDefin.hpp"
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

		if (registry.ctx<GameMode>() == GameMode::kClinet)
		{
			// ���͸���
			for (auto e : view)
			{
				// �жϸ���
			}
		}
		else
		{
			// ����ȫ��
			for (auto e : view)
			{

			}
		}

		// todo;
		//registry.ctx<Locator>().Get<NetworkService>()->Send(nullptr, 0);
	}

	void Finalize() override
	{

	}
};