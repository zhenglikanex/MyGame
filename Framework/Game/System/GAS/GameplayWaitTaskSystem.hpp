#pragma once

#include <functional>

#include "Framework/Game/Component/EntityByUniqueIdMap.hpp"
#include "Framework/Game/Component/Destory.hpp"

#include "Framework/Game/Component/GAS/GameplayAbilityScript.hpp"
#include "Framework/Game/Component/GAS/GameplayWaitTask.hpp"

#include "Framework/Game/System.hpp"

#include "3rdparty/include/sol/sol.hpp"

struct GameplayWaitTaskSystem : System
{
public:
	std::vector<uint32_t> del_list;

	GameplayWaitTaskSystem(entt::registry& _registry) : System(_registry)
	{
	}

	~GameplayWaitTaskSystem() {}

	bool Initialize() override
	{
		return true;
	}

	void FixedUpdate(float dt) override
	{
		float time = static_cast<float>(dt);

		auto view = registry.view<GameplayAbilityScript,GameplayWaitTask>(entt::exclude<Destory>);
		for (auto e : view)
		{
			auto& [script,task] = registry.get<GameplayAbilityScript,GameplayWaitTask>(e);

			uint32_t size = task.value.size();
			for (uint32_t i = 0; i < size; ++i)
			{
				auto& spec = task.value[i];
				spec.time -= time;
				if (spec.time <= 0)
				{
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