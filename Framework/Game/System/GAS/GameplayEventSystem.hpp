#pragma once

#include <functional>

#include "Framework/Game/Component/EntityByLocalMap.hpp"
#include "Framework/Game/Component/GAS/GameplayEvent.hpp"
#include "Framework/Game/Component/GAS/GameplayAbilityScript.hpp"

#include "Framework/Game/Utility/GameplayAbilityUtility.hpp"

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

		if (auto* manager = registry.try_ctx<GameplayEventSendManager>())
		{
			auto& map = registry.ctx<EntityByLocalMap>();
			for (auto& sender : manager->senders)
			{
				auto it = map.value.find(sender.first);
				if (it != map.value.end())
				{
					entt::entity receiver = it->second;
					
					if (!registry.all_of<UniqueId>(receiver))
					{
						continue;
					}

					auto view = registry.view<GameplayAbilityScript>();
					auto abilities = GameplayAbilityUtility::GetActiveGameplayAbilities(registry, registry.get<UniqueId>(receiver));
					for (auto& event : sender.second)
					{
						for (auto ability : abilities)
						{
							if (auto* task = registry.try_get<GameplayWaitEventTask>(ability))
							{
								auto it = task->value.find(event.tag);
								if (it != task->value.end())
								{
									auto& script = view.get<GameplayAbilityScript>(ability);
									script.value[it->second.handler](script.value, event.data);
								}
							}

						}
					}
					
				}
			}

			manager->senders.clear();
		}
	}

	void Finalize() override
	{

	}
};