#pragma once

#include "Framework/Game/Component/Destroy.hpp"
#include "Framework/Game/Component/AnimationClip.hpp"
#include "Framework/Game/Component/GAS/GameplayAbilitySpec.hpp"
#include "Framework/Game/Component/GAS/GameplayAbilityScript.hpp"
#include "Framework/Game/Component/GAS/GameplayAnimWaitTask.hpp"

#include "Framework/Game/System.hpp"

#include "3rdparty/include/sol/sol.hpp"

struct GameplayAnimWaitTaskSystem : System
{
public:
	GameplayAnimWaitTaskSystem(Registry& _registry) : System(_registry)
	{
	}

	bool Initialize() override
	{
		return true;
	}

	void FixedUpdate(float dt) override
	{
		auto view = registry.view<GameplayAbilitySpec,GameplayAbilityScript,GameplayAnimWaitTask>();
		for (auto e : view)
		{
			auto& [spec, script, task] = registry.get<GameplayAbilitySpec, GameplayAbilityScript, GameplayAnimWaitTask>(e);
			
			if (registry.valid(spec.source))
			{
				auto source = registry.get(spec.source);
				auto& animation_clip = registry.get<AnimationClip>(source);

				if(!registry.all_of<AnimationClip>(source))
				{
					continue;
				}

				auto& task_spec = task;
				bool completed = false;

				if (!task_spec.running)
				{
					task_spec.running = true;
					task_spec.animation = animation_clip.name;
				}

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

				if(completed)
				{
					task_spec.running = false;
				}
			}
		}
	}

	void Finalize() override
	{

	}
};