#pragma once

#include "Framework/Game/RandomEngine.hpp"

#include "Framework/Game/GAS/GameplayEffectManager.hpp"

#include "Framework/Game/Component/GAS/ApplyGameplayEffectList.hpp"
#include "Framework/Game/Component/GAS/GameplayEffectList.hpp"
#include "Framework/Game/Component/GAS/GameplayTags.hpp"

#include "Framework/Game/System.hpp"

struct ExecuteGameplayEffectSystem : public System
{

	ExecuteGameplayEffectSystem(entt::registry& _registry) : System(_registry) { }
	~ExecuteGameplayEffectSystem() {}

	bool Initialize() override
	{
		return true;
	}

	void FixedUpdate(fixed16 dt) override
	{
		float time = static_cast<float>(dt);
		auto view = registry.view<GameplayEffectList, GameplayTags>();
		for (auto e : view)
		{
			auto& [ge_list, tags] = view.get<GameplayEffectList, GameplayTags>(e);
			for (auto& spec : ge_list.value)
			{
				spec.duration += time;
				spec.period += time;

				// ÒÆ³ý
				if (spec.duration >= spec.gameplay_effect.period.duration)
				{
					//todo:
				}

				if (!CheckOngoingTagRequirements(spec, tags))
				{
					return;
				}

				if (spec.period >= spec.gameplay_effect.period.period)
				{
					//todo modifiers
				}
				
				
			}
		}
	}

	bool CheckOngoingTagRequirements(const GameplayEffectSpec& spec,const GameplayTags& tags)
	{
		if (!tags.HasAll(spec.gameplay_effect.tag.ongoing_tag_requirements.require_tags) ||
			tags.HasAny(spec.gameplay_effect.tag.ongoing_tag_requirements.ignore_tags))
		{
			return false;
		}

		return true;
	}

	void Modify()
	{

	}

	void Finalize() override
	{

	}
};
