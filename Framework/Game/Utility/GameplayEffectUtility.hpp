#pragma once

#include "Framework/Game/GAS/GameplayEffect.hpp"

#include "Framework/Game/Component/GAS/ApplyGameplayEffect.hpp"
#include "Framework/Game/Component/GAS/ModifierAttribute.hpp"

#include "3rdparty/include/entt/entt.hpp"

class GameplayEffectUtility
{
public:
	static std::vector<entt::entity> GetGameplayEffectSpecEntitiesByOwner(entt::registry& registry,entt::entity owner)
	{
		std::vector<entt::entity> entities;
		
		auto view = registry.view<GameplayEffectSpec>();
		for (auto e : view)
		{
			const auto& spec = view.get<GameplayEffectSpec>(e);
			if (spec.owner == owner)
			{
				entities.push_back(e);
			}
		}

		return entities;
	}

	static void ApplyModifiers(entt::registry& registry,const GameplayEffectSpec& spec)
	{
		//is modify base
		if (spec.ge->gameplay_effect.duration_policy == GameplayEffect::DurationPolicy::Instant ||
			spec.ge->gameplay_effect.duration_policy == GameplayEffect::DurationPolicy::Duration && spec.ge->period.period > 0)
		{
			auto& mod_base_attr_set = registry.get_or_emplace<ModifierBaseAttributeSet>(spec.owner);
			
			for (auto& mod : spec.ge->gameplay_effect.modifiers)
			{
				float value = GetModifyValue(mod.modifier_magnitude);
				mod_base_attr_set.value[mod.attribute].emplace_back(ModifierAttribute{ mod.modifier_op,value });
			}
		}
		else
		{
			auto& mod_attr_set = registry.get_or_emplace<ModifierAttributeSet>(spec.owner);
			for (auto& mod : spec.ge->gameplay_effect.modifiers)
			{
				float value = GetModifyValue(mod.modifier_magnitude);
				mod_attr_set.value[mod.attribute].emplace_back(ModifierAttribute{ mod.modifier_op,value });
			}
		}		
	}

	static void ApplyGameEffectToTarget(entt::registry& registry, entt::entity source, std::string_view effect_name, uint32_t level, entt::entity target)
	{
		auto& apply_ge = registry.ctx_or_set<ApplyGameplayEffect>();
		auto it = apply_ge.value.find(source);
		if (it == apply_ge.value.end())
		{
			apply_ge.value.emplace(source, std::vector<GameplayEffectInfo>());
		}
		apply_ge.value[source].emplace_back(effect_name, level, source, target);
	}

private:
	static float GetModifyValue(const GameplayEffect::ModifierMagnitude& magnitude)
	{
		float value = 0;
		if (magnitude.calculation_type == GameplayEffect::ModifierMagnitudeCalculationType::ScalableFloat)
		{
			// todo :
			value = magnitude.value;
		}
		else if (magnitude.calculation_type == GameplayEffect::ModifierMagnitudeCalculationType::AttributeBased)
		{
			// todo :
			value = magnitude.value;
		}
		else if (magnitude.calculation_type == GameplayEffect::ModifierMagnitudeCalculationType::CustomCalculationClass)
		{
			// todo :
			value = magnitude.value;
		}

		return value;
	}
};