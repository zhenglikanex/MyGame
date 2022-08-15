#pragma once

#include "Framework/Game/RandomEngine.hpp"

#include "Framework/Game/GAS/GameplayEffectManager.hpp"

#include "Framework/Game/Component/GAS/ApplyGameplayEffect.hpp"
#include "Framework/Game/Component/GAS/GameplayEffectSpec.hpp"

#include "Framework/Game/Utility/GameplayEffectUtility.hpp"

#include "Framework/Game/System.hpp"

struct GameplayEffectSystem : public System
{
	GameplayEffectSystem(Registry& _registry) : System(_registry) { }
	~GameplayEffectSystem() {}

	bool Initialize() override
	{
		return true;
	}

	void FixedUpdate(float dt) override
	{
		auto& ge_manager = registry.ctx<GameplayEffectManager>();

		auto view = registry.view<Local,GameplayEffectSpec>(entt::exclude<Destroy>);
		for (auto e : view)
		{
			auto& spec = view.get<GameplayEffectSpec>(e);
			
			if (!registry.valid(spec.target))
			{
				registry.emplace<Destroy>(e);
				continue;
			}

			entt::entity target = registry.get(spec.target);

			spec.time += dt;

			auto& ge = ge_manager.LoadGameplayEffect(spec.effect);

			if (ge.period.period > 0)
			{
				spec.period += dt;

				if (spec.period >= ge.period.period)
				{
					if (!CheckOngoingTagRequirements(target,ge))
					{
						GameplayEffectUtility::ApplyExecutions(registry, spec);
						GameplayEffectUtility::ApplyModifiers(registry, spec);
					}

					spec.period -= ge.period.period;
				}
			}

			// ÒÆ³ý
			if (spec.time >= spec.duration && ge.gameplay_effect.duration_policy == GameplayEffect::DurationPolicy::Duration)
			{
				if (spec.stack_count > 1 && ge.stacking.statck_expiration_policy == GameplayEffect::StatckExpirationPolicy::RemoveSingleStackAndRefreshDuration)
				{
					--spec.stack_count;
					spec.time = 0;
				}
				else
				{
					registry.emplace<Destroy>(e);
				}
			}
		}
	}

	bool CheckOngoingTagRequirements(entt::entity target,const GameplayEffect& ge)
	{
		auto& tag_count = registry.get<GameplayTagCountContainer>(target);
		return tag_count.HasAll(ge.tag.application_tag_requirements.require_tags) && !tag_count.HasAny(ge.tag.application_tag_requirements.ignore_tags);
	}

	void Finalize() override
	{
		
	}
};
