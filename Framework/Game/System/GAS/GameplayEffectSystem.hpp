#pragma once

#include "Framework/Game/RandomEngine.hpp"

#include "Framework/Game/GAS/GameplayEffectManager.hpp"

#include "Framework/Game/Component/GAS/ApplyGameplayEffect.hpp"
#include "Framework/Game/Component/GAS/GameplayEffectSpec.hpp"
#include "Framework/Game/Component/GAS/GameplayTags.hpp"

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

		auto view = registry.view<GameplayEffectSpec>(entt::exclude<Destory>);
		for (auto e : view)
		{
			auto& spec = view.get<GameplayEffectSpec>(e);
			
			if (!registry.valid(spec.target))
			{
				registry.emplace<Destory>(e);
				continue;
			}

			entt::entity target = registry.get(spec.target);

			spec.duration += dt;

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
			if (spec.duration >= ge.period.duration && ge.gameplay_effect.duration_policy == GameplayEffect::DurationPolicy::Duration)
			{
				if (spec.stack_count > 1 && ge.stacking.statck_expiration_policy == GameplayEffect::StatckExpirationPolicy::RemoveSingleStackAndRefreshDuration)
				{
					--spec.stack_count;
					spec.duration = 0;
				}
				else
				{
					registry.emplace<Destory>(e);
				}
			}
		}
	}

	bool CheckOngoingTagRequirements(entt::entity target,const GameplayEffect& ge)
	{
		auto tags = registry.try_get<GameplayTags>(target);

		if(!tags && !ge.tag.application_tag_requirements.require_tags.Empty())
		{
			return false;
		}

		if (!tags)
		{
			return true;
		}

		return tags->tags.HasAll(ge.tag.application_tag_requirements.require_tags) && !tags->tags.HasAny(ge.tag.application_tag_requirements.ignore_tags);
	}

	void Finalize() override
	{
		
	}
};
