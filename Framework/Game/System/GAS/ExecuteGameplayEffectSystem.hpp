#pragma once

#include "Framework/Game/RandomEngine.hpp"

#include "Framework/Game/GAS/GameplayEffectManager.hpp"

#include "Framework/Game/Component/GAS/ApplyGameplayEffect.hpp"
#include "Framework/Game/Component/GAS/GameplayEffectSpec.hpp"
#include "Framework/Game/Component/GAS/GameplayTags.hpp"

#include "Framework/Game/Utility/GameplayEffectUtility.hpp"

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
		auto view = registry.view<GameplayEffectSpec>();
		for (auto e : view)
		{
			auto& spec = view.get<GameplayEffectSpec>(e);
			if (!registry.valid(spec.owner))
			{
				registry.emplace<Destory>(e);
				continue;
			}

			spec.duration += time;
			spec.period += time;

			if (spec.ge->period.period > 0)
			{
				// ÒÆ³ý
				if (spec.duration >= spec.ge->period.duration)
				{
					//todo:
				}

				if (!CheckOngoingTagRequirements(spec))
				{
					return;
				}

				if (spec.period >= spec.ge->period.period)
				{
					GameplayEffectUtility::ApplyModifiers(registry, spec);
					spec.period -= spec.ge->period.period;
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

	void Modify()
	{
		
	}

	void Finalize() override
	{
		
	}
};
