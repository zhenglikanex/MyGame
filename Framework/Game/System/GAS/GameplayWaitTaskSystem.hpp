#pragma once

#include <functional>

#include "Framework/Game/Component/EntityByUniqueIdMap.hpp"
#include "Framework/Game/Component/Destroy.hpp"

#include "Framework/Game/Component/GAS/GameplayAbilityScript.hpp"
#include "Framework/Game/Component/GAS/GameplayWaitTask.hpp"

#include "Framework/Game/System.hpp"

#include "3rdparty/include/sol/sol.hpp"

struct GameplayWaitTaskSystem : System
{
public:
	std::vector<uint32_t> del_list;

	GameplayWaitTaskSystem(Registry& _registry) : System(_registry)
	{
	}

	~GameplayWaitTaskSystem() {}

	bool Initialize() override
	{
		return true;
	}

	void FixedUpdate(float dt) override
	{
		auto view = registry.view<GameplayAbilityScript,GameplayWaitTask>(entt::exclude<Destroy>);
		for (auto e : view)
		{
			auto& [script,task] = registry.get<GameplayAbilityScript,GameplayWaitTask>(e);

			uint32_t size = task.value.size();
			for (uint32_t i = 0; i < size; ++i)
			{
				auto& spec = task.value[i];
				spec.time -= dt;
				if (spec.time <= 0)
				{
					auto& ability_spec = registry.get<GameplayAbilitySpec>(e);
					script.value[spec.completed](script.value);
					del_list.emplace_back(i);	
				}
			}

			for (int i = del_list.size() - 1; i >= 0; --i)
			{
				auto index = del_list[i];
				task.value.erase(task.value.begin() + index);
			}

			del_list.clear();
		}
	}

	void Finalize() override
	{

	}
};