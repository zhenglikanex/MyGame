#pragma once

#include "Framework/Game/Component/EntityByUniqueIdMap.hpp"
#include "Framework/Game/Component/Command.hpp"
#include "Framework/Game/Component/GAS/GameplayAbilitySpec.hpp"
#include "Framework/Game/Component/GAS/GameplayAbilityScript.hpp"
#include "Framework/Game/Component/GAS/GameplayInputTask.hpp"

#include "Framework/Game/System.hpp"

#include "3rdparty/include/sol/sol.hpp"

struct GameplayPlayInputSystem : System
{
public:
	GameplayPlayInputSystem(entt::registry& _registry) : System(_registry)
	{
	}

	~GameplayPlayInputSystem() {}

	bool Initialize() override
	{
		return true;
	}

	void FixedUpdate(fixed16 dt) override
	{
		float time = static_cast<float>(dt);

		auto map = registry.ctx<EntityByUniqueIdMap>();
		auto view = registry.view<GameplayAbilitySpec, GameplayAbilityScript, GameplayInputTask>();
		for (auto e : view)
		{
			auto& [spec, script, task] = registry.get<GameplayAbilitySpec, GameplayAbilityScript,GameplayInputTask>(e);
			auto it = map.value.find(spec.source);
			if (it != map.value.end())
			{
				entt::entity source = it->second;

				auto it = task.value.begin();
				while (it != task.value.end())
				{
					auto& task_spec = it->second;
					bool completed = false;

					auto commond = registry.try_get<Command>(source);
					if (commond)
					{
						if (!task_spec.is_pressed && commond->key == task_spec.key && task_spec.pressed != "")
						{
							script.value[task_spec.pressed](script.value);

							// 如果没有监听按钮释放,可以提前结束task
							if (task_spec.released == "")
							{
								completed = true;
							}
						}

						if (task_spec.is_pressed && commond->key != task_spec.key && task_spec.released != "")
						{
							script.value[task_spec.released](script.value);

							completed = true;
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