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

			spec.duration += time;
			spec.period += time;

			// ÒÆ³ý
			if (spec.duration >= spec.ge.period.duration)
			{
				//todo:
			}

			if (!CheckOngoingTagRequirements(spec))
			{
				return;
			}

			if (spec.period >= spec.ge.period.period)
			{
				GameplayEffectUtility::ApplyModifiers(registry, spec);
			}
		}
	}

	bool CheckOngoingTagRequirements(const GameplayEffectSpec& spec)
	{
		auto owner_tags = registry.get<GameplayTags>(spec.owner);

		if (!owner_tags.HasAll(spec.ge.tag.ongoing_tag_requirements.require_tags) ||
			owner_tags.HasAny(spec.ge.tag.ongoing_tag_requirements.ignore_tags))
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
