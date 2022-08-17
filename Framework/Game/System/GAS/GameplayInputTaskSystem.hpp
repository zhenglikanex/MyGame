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

	bool Initialize() override
	{
		return true;
	}

	void FixedUpdate(float dt) override
	{
		auto& command = registry.ctx_or_set<GameplayAbilityCommand>();
		if (command.value.empty())
		{
			return;
		}

		auto view = registry.view<GameplayAbilitySpec, GameplayAbilityScript, GameplayInputTask>();
		for (auto e : view)
		{
			auto& [spec, script, task] = registry.get<GameplayAbilitySpec, GameplayAbilityScript,GameplayInputTask>(e);
			if (registry.valid(spec.source))
			{
				auto command_it = command.value.find(spec.source);
				if(command_it == command.value.end())
				{
					continue;
				}

				std::unordered_map<std::string,GameplayAbilityCommandSpec>& ability_commands = command_it->second;

				auto it = task.value.begin();
				while (it != task.value.end())
				{
					auto& task_spec = it->second;
					bool completed = false;

					if (!command.value.empty())
					{
						auto ability_it = ability_commands.find(task_spec.key);
						if (ability_it != ability_commands.end())
						{
							if (ability_it->second.activated)
							{
								script.value[task_spec.pressed](script.value);

								// 如果没有监听按钮释放,可以提前结束task
								if (task_spec.released.empty())
								{
									completed = true;
								}
							}
							else if (!ability_it->second.activated)
							{
								if (!task_spec.released.empty())
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