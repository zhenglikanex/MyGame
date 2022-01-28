#pragma once

#include "Framework/Game/Component/EntityByUniqueIdMap.hpp"
#include "Framework/Game/Component/Destory.hpp"
#include "Framework/Game/Component/AnimationClip.hpp"
#include "Framework/Game/Component/GAS/GameplayAbilitySpec.hpp"
#include "Framework/Game/Component/GAS/GameplayAbilityScript.hpp"
#include "Framework/Game/Component/GAS/GameplayPlayAnimAndWaitTask.hpp"

#include "Framework/Game/System.hpp"

#include "3rdparty/include/sol/sol.hpp"

struct GameplayPlayAnimAndWaitTaskSystem : System
{
public:
	GameplayPlayAnimAndWaitTaskSystem(entt::registry& _registry) : System(_registry)
	{
	}

	~GameplayPlayAnimAndWaitTaskSystem() {}

	bool Initialize() override
	{
		return true;
	}

	void FixedUpdate(float dt) override
	{
		float time = static_cast<float>(dt);

		auto map = registry.ctx<EntityByUniqueIdMap>();
		auto view = registry.view<GameplayAbilitySpec,GameplayAbilityScript,GameplayPlayAnimAndWaitTask>();
		for (auto e : view)
		{
			auto& [spec, script, task] = registry.get<GameplayAbilitySpec, GameplayAbilityScript, GameplayPlayAnimAndWaitTask>(e);
			auto it = map.value.find(spec.source);
			if (it != map.value.end())
			{
				auto source = it->second;

				auto it = task.value.begin();
				while (it != task.value.end())
				{
					auto& task_spec = *it;
					bool completed = false;

					if (!task_spec.running)
					{
						task_spec.running = true;
						registry.replace<AnimationClip>(source,task_spec.animation);
					}

					auto& animation_clip = registry.get<AnimationClip>(source);
					if (animation_clip.name != task_spec.animation)
					{
						if (task_spec.canceled != "")
						{
							script.value[task_spec.canceled](script.value);
						}
						completed = true;
					}
					else if (animation_clip.is_done)
					{
						if (task_spec.completed != "")
						{
							script.value[task_spec.completed](script.value);
						}
						completed = true;
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