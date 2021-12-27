#pragma once

#include "Framework/Game/RandomEngine.hpp"

#include "Framework/Game/GAS/GameplayEffectManager.hpp"

#include "Framework/Game/Component/EntityByUniqueIdMap.hpp"
#include "Framework/Game/Component/GAS/ApplyGameplayEffect.hpp"
#include "Framework/Game/Component/GAS/GameplayEffectSpec.hpp"
#include "Framework/Game/Component/GAS/GameplayTags.hpp"

#include "Framework/Game/Utility/GameplayEffectUtility.hpp"

#include "Framework/Game/System.hpp"

struct GameplayEffectSystem : public System
{

	GameplayEffectSystem(entt::registry& _registry) : System(_registry) { }
	~GameplayEffectSystem() {}

	bool Initialize() override
	{
		return true;
	}

	void FixedUpdate(fixed16 dt) override
	{
		auto& e_by_unique_id_map = registry.ctx<EntityByUniqueIdMap>();
		auto& ge_manager = registry.ctx<GameplayEffectManager>();

		float time = static_cast<float>(dt);
		auto view = registry.view<GameplayEffectSpec>(entt::exclude<Destory>);
		for (auto e : view)
		{
			auto& spec = view.get<GameplayEffectSpec>(e);
			
			auto it = e_by_unique_id_map.value.find(spec.owner);
			if (it == e_by_unique_id_map.value.end() || !registry.valid(it->second))
			{
				registry.emplace<Destory>(e);
				continue;
			}

			spec.duration += time;

			auto& ge = ge_manager.LoadGameplayEffect(spec.effect);

			if (ge.period.period > 0)
			{
				spec.period += time;

				if (spec.period >= ge.period.period)
				{
					if (!CheckOngoingTagRequirements(it->second,ge))
					{
						GameplayEffectUtility::ApplyModifiers(registry, ge, it->second, spec.level);
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

	bool CheckOngoingTagRequirements(entt::entity owner,const GameplayEffect& ge)
	{
		auto owner_tags = registry.try_get<GameplayTags>(owner);

		if(!owner_tags && !ge.tag.application_tag_requirements.require_tags.Empty())
		{
			return false;
		}

		if (!owner_tags)
		{
			return true;
		}

		return owner_tags->tags.HasAll(ge.tag.application_tag_requirements.require_tags) && !owner_tags->tags.HasAny(ge.tag.application_tag_requirements.ignore_tags);
	}

	void Finalize() override
	{
		
	}
};
