#pragma once

#include "Framework/Game/Component/Command.hpp"
#include "Framework/Game/Component/GAS/GameplayAbilitySpec.hpp"
#include "Framework/Game/Component/GAS/GameplayAbilityScript.hpp"
#include "Framework/Game/Component/GAS/GameplayInputTask.hpp"

#include "Framework/Game/System.hpp"

#include "3rdparty/include/sol/sol.hpp"

struct GameplayPlayInputSystem : System
{
public:
	GameplayPlayInputSystem(Registry& _registry) : System(_registry)
	{
	}

	~GameplayPlayInputSystem() {}

	bool Initialize() override
	{
		return true;
	}

	void FixedUpdate(float dt) override
	{
		
		auto view = registry.view<GameplayAbilitySpec, GameplayAbilityScript, GameplayInputTask>();
		for (auto e : view)
		{
			auto& [spec, script, task] = registry.get<GameplayAbilitySpec, GameplayAbilityScript,GameplayInputTask>(e);
			if (registry.valid(spec.source))
			{
				entt::entity source = registry.get(spec.source);

				auto it = task.value.begin();
				while (it != task.value.end())
				{
					auto& task_spec = it->second;
					bool completed = false;

					auto commond = registry.try_get<Command>(source);
					if (commond)
					{
						auto it = commond->abilities.find(task_spec.key);
						if (it != commond->abilities.end())
						{
							if (it->second == AbilityStatus::kDown)
							{
								script.value[task_spec.pressed](script.value);

								// 如果没有监听按钮释放,可以提前结束task
								if (task_spec.released == "")
								{
									completed = true;
								}
							}
							else if (it->second == AbilityStatus::kUp)
							{
								if (task_spec.released != "")
								{
									script.value[task_spec.released](script.value);

									completed = true;
								}
							}
						}
					}

					if (completed)
					{
						it = task.value.erase(it);
					}
					else
					{
						++it;
					}
				}
			}
		}
		
	}

	void Finalize() override
	{

	}
};