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

	GameplayEffectSystem(entt::registry& _registry) : System(_registry) { }
	~GameplayEffectSystem() {}

	bool Initialize() override
	{
		return true;
	}

	void FixedUpdate(fixed16 dt) override
	{
		float time = static_cast<float>(dt);
		auto view = registry.view<GameplayEffectSpec>(entt::exclude<Destory>);
		for (auto e : view)
		{
			auto& spec = view.get<GameplayEffectSpec>(e);

			if (!registry.valid(spec.owner))
			{
				registry.emplace<Destory>(e);
				continue;
			}

			spec.duration += time;

			if (spec.ge->period.period > 0)
			{
				spec.period += time;

				if (spec.period >= spec.ge->period.period)
				{
					if (!CheckOngoingTagRequirements(spec))
					{
						GameplayEffectUtility::ApplyModifiers(registry, spec);
					}

					spec.period -= spec.ge->period.period;
				}
			}

			// ÒÆ³ý
			if (spec.duration >= spec.ge->period.duration && spec.ge->gameplay_effect.duration_policy == GameplayEffect::DurationPolicy::Duration)
			{
				if (spec.stack_count > 1 && spec.ge->stacking.statck_expiration_policy == GameplayEffect::StatckExpirationPolicy::RemoveSingleStackAndRefreshDuration)
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

	bool CheckOngoingTagRequirements(const GameplayEffectSpec& spec)
	{
		auto owner_tags = registry.try_get<GameplayTags>(spec.owner);

		if(!owner_tags && !spec.ge->tag.application_tag_requirements.require_tags.Empty())
		{
			return false;
		}

		if (!owner_tags)
		{
			return true;
		}

		return owner_tags->tags.HasAll(spec.ge->tag.application_tag_requirements.require_tags) && !owner_tags->tags.HasAny(spec.ge->tag.application_tag_requirements.ignore_tags);
	}

	void Finalize() override
	{
		
	}
};
