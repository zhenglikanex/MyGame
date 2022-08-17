#pragma once

#include <functional>

#include "Framework/Game/Component/GAS/GameplayEvent.hpp"
#include "Framework/Game/Component/GAS/GameplayAbilityScript.hpp"

#include "Framework/Game/Utility/GameplayAbilityUtility.hpp"

#include "Framework/Game/System.hpp"

#include "3rdparty/include/sol/sol.hpp"

struct GameplayEventSystem : System
{
public:
	GameplayEventSystem(Registry& _registry) : System(_registry)
	{
	}

	bool Initialize() override
	{
		return true;
	}

	void FixedUpdate(float dt) override
	{
		if (auto* manager = registry.try_ctx<GameplayEventSendManager>())
		{
			for (auto& sender : manager->senders)
			{
				entt::entity receiver = sender.first;
				if (registry.valid(receiver))
				{
					if (!registry.all_of<Remote>(receiver))
					{
						continue;
					}

					auto view = registry.view<GameplayAbilityScript>();
					auto abilities = gameplay_ability_utility::GetActiveGameplayAbilities(registry, registry.get<Remote>(receiver));
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