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
	GameplayWaitTaskSystem(entt::registry& _registry) : System(_registry)
	{
	}

	~GameplayWaitTaskSystem() {}

	bool Initialize() override
	{
		return true;
	}

	void FixedUpdate(fixed16 dt) override
	{
		float time = static_cast<float>(dt);

		auto view = registry.view<GameplayAbilityScript,GameplayWaitTask>(entt::exclude<Destory>);
		for (auto e : view)
		{
			auto& [script,task] = registry.get<GameplayAbilityScript,GameplayWaitTask>(e);

			auto it = task.value.begin();
			while (it != task.value.end())
			{
				auto& spec = *it;
				spec.time -= time;
				if (spec.time <= 0)
				{
					script.value[spec.completed](script.value);
					it = task.value.erase(it);
				}
				else
				{
					++it;
				}
			}

		}
	}

	void Finalize() override
	{

	}
};