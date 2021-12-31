#pragma once

#include "Framework/Game/GAS/GameplayEffect.hpp"

#include "Framework/Game/Component/Destory.hpp"

#include "Framework/Game/Component/GAS/ApplyGameplayEffect.hpp"
#include "Framework/Game/Component/GAS/ModifierAttribute.hpp"
#include "Framework/Game/Component/GAS/GameplayEffectSpec.hpp"

#include "3rdparty/include/entt/entt.hpp"

class GameplayEffectUtility
{
public:
	static std::vector<entt::entity> GetGameplayEffectSpecEntitiesByOwner(entt::registry& registry,UniqueId owner)
	{
		std::vector<entt::entity> entities;
		
		auto view = registry.view<GameplayEffectSpec>(entt::exclude<Destory>);
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

	static void ApplyModifiers(entt::registry& registry,const GameplayEffect& ge,entt::entity target,uint32_t level)
	{
		//is modify base
		if (ge.gameplay_effect.duration_policy == GameplayEffect::DurationPolicy::Instant ||
			ge.gameplay_effect.duration_policy == GameplayEffect::DurationPolicy::Duration && ge.period.period > 0)
		{
			auto& mod_base_attr_set = registry.get<ModifierBaseAttributeSet>(target);
			for (auto& mod : ge.gameplay_effect.modifiers)
			{
				float value = GetModifyValue(mod.modifier_magnitude);
				registry.patch<ModifierBaseAttributeSet>(target, [&mod,value](ModifierBaseAttributeSet& mod_base_attr_set)
					{
						mod_base_attr_set.value[mod.attribute].emplace_back(ModifierAttribute{ mod.modifier_op,value });
					});
				
			}
		}
		else
		{
			auto& mod_attr_set = registry.get<ModifierAttributeSet>(target);
			for (auto& mod : ge.gameplay_effect.modifiers)
			{
				float value = GetModifyValue(mod.modifier_magnitude);
				registry.patch<ModifierAttributeSet>(target, [&mod, value](ModifierAttributeSet& mod_attr_set)
					{
						mod_attr_set.value[mod.attribute].emplace_back(ModifierAttribute{ mod.modifier_op,value });
					});
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