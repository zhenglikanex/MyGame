#pragma once

#include <functional>

#include "Framework/Game/Component/GAS/GameplayEvent.hpp"

#include "Framework/Game/System.hpp"

#include "3rdparty/include/sol/sol.hpp"

struct GameplayEventSystem : System
{
public:
	GameplayEventSystem(entt::registry& _registry) : System(_registry)
	{
	}
	~GameplayEventSystem() {}

	bool Initialize() override
	{
		return true;
	}

	void FixedUpdate(fixed16 dt) override
	{
		float time = static_cast<float>(dt);

		auto view = registry.view<GameplayEventReceiverMap>();
		if (auto* sender = registry.try_ctx<GameplayEventSender>())
		{
			for (auto& event : sender->events)
			{
				for (auto e : view)
				{
					auto& map = view.get<GameplayEventReceiverMap>(e);
					auto it = map.value.find(event.tag);
					if (it != map.value.end())
					{
						it->second.callback(event.data);
					}

					if (map.value.empty())
					{
						registry.remove<GameplayEventReceiverMap>(e);
					}
				}
			}

			sender->events.clear();
		}
	}

	void Finalize() override
	{

	}
};