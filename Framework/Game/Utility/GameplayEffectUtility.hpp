#pragma once

#include "Framework/Game/GAS/GameplayEffect.hpp"

#include "Framework/Game/Component/GAS/ModifierAttribute.hpp"

#include "3rdparty/include/entt/entt.hpp"

class GameplayEffectUtility
{
public:
	void ApplyModifiers(entt::registry& registry,const GameplayEffectSpec& spec)
	{
		entt::entity target = entt::null;

		//is modify base
		if (spec.gameplay_effect.gameplay_effect.duration_policy == GameplayEffect::DurationPolicy::Instant ||
			spec.gameplay_effect.gameplay_effect.duration_policy == GameplayEffect::DurationPolicy::Duration && spec.gameplay_effect.period.period > 0)
		{
			auto& mod_base_attr_set = registry.get_or_emplace<ModifierBaseAttributeSet>(target);
			
			for (auto& mod : spec.gameplay_effect.gameplay_effect.modifiers)
			{
				float value = 0;
				if (mod.modifier_magnitude.calculation_type == GameplayEffect::ModifierMagnitudeCalculationType::ScalableFloat)
				{
					// todo :
					value = mod.modifier_magnitude.value;
				}
				else if(mod.modifier_magnitude.calculation_type == GameplayEffect::ModifierMagnitudeCalculationType::AttributeBased)
				{
					// todo :
					value = mod.modifier_magnitude.value;
				}
				else if(mod.modifier_magnitude.calculation_type == GameplayEffect::ModifierMagnitudeCalculationType::CustomCalculationClass)
				{
					// todo :
					value = mod.modifier_magnitude.value;
				}

				mod_base_attr_set.value[mod.attribute].emplace_back(ModifierAttribute{ mod.modifier_op,value });
			}
		}
		else
		{
			
		}
		
	}
};