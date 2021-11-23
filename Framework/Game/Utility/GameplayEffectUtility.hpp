#pragma once

#include "Framework/Game/GAS/GameplayEffect.hpp"

#include "Framework/Game/Component/GAS/ModifierAttribute.hpp"

#include "3rdparty/include/entt/entt.hpp"

class GameplayEffectUtility
{
public:
	static void ApplyModifiers(entt::registry& registry,const GameplayEffectSpec& spec,entt::entity target)
	{
		//is modify base
		if (spec.gameplay_effect.gameplay_effect.duration_policy == GameplayEffect::DurationPolicy::Instant ||
			spec.gameplay_effect.gameplay_effect.duration_policy == GameplayEffect::DurationPolicy::Duration && spec.gameplay_effect.period.period > 0)
		{
			auto& mod_base_attr_set = registry.get_or_emplace<ModifierBaseAttributeSet>(target);
			
			for (auto& mod : spec.gameplay_effect.gameplay_effect.modifiers)
			{
				float value = GetModifyValue(mod.modifier_magnitude);
				mod_base_attr_set.value[mod.attribute].emplace_back(ModifierAttribute{ mod.modifier_op,value });
			}
		}
		else
		{
			auto& mod_attr_set = registry.get<ModifierAttributeSet>(target);
			for (auto& mod : spec.gameplay_effect.gameplay_effect.modifiers)
			{
				float value = GetModifyValue(mod.modifier_magnitude);
				mod_attr_set.value[mod.attribute].emplace_back(ModifierAttribute{ mod.modifier_op,value });
			}
		}		
	}
private:
	static float GetModifyValue(GameplayEffect::ModifierMagnitude& magnitude)
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