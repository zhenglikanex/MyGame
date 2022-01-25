#pragma once

#include "Framework/Game/GAS/GameplayEffectManager.hpp"
#include "Framework/Game/GAS/GameplayEffect.hpp"
#include "Framework/Game/GAS/GameplayEffectExecutionCalculation.hpp"

#include "Framework/Game/Component/EntityByUniqueIdMap.hpp"
#include "Framework/Game/Component/Destory.hpp"
#include "Framework/Game/Component/GAS/ApplyGameplayEffect.hpp"
#include "Framework/Game/Component/GAS/ModifierAttribute.hpp"
#include "Framework/Game/Component/GAS/GameplayEffectSpec.hpp"

#include "3rdparty/include/entt/entt.hpp"

class GameplayEffectUtility
{
public:
	static GameplayEffectExecutionCalculation* MakeExecutionCalculation(std::string_view class_name)
	{
		if (class_name == "RpgExecutionCalculation")
		{
			static RpgExecutionCalculation execution;
			return &execution;
		}

		assert("not GameplayEffectExecutionCalculation" && false);

		return nullptr;
	}

	static std::vector<entt::entity> GetGameplayEffectSpecEntitiesByOwner(entt::registry& registry,UniqueId target)
	{
		std::vector<entt::entity> entities;
		
		auto view = registry.view<GameplayEffectSpec>(entt::exclude<Destory>);
		for (auto e : view)
		{
			const auto& spec = view.get<GameplayEffectSpec>(e);
			if (spec.target == target)
			{
				entities.push_back(e);
			}
		}

		return entities;
	}

	static void ApplyExecutions(entt::registry& registry, const GameplayEffectSpec& spec)
	{
		auto& ge_manager = registry.ctx<GameplayEffectManager>();
		auto& ge = ge_manager.LoadGameplayEffect(spec.effect);

		if (ge.gameplay_effect.duration_policy == GameplayEffect::DurationPolicy::Instant ||
			ge.gameplay_effect.duration_policy == GameplayEffect::DurationPolicy::Duration && ge.period.period > 0)
		{
			for (auto& execution : ge.gameplay_effect.executions)
			{
				auto execut = GameplayEffectUtility::MakeExecutionCalculation(execution.class_name);
				execut->ExecutionCalculation(registry,spec);
			}
		}
	}

	static void ApplyModifiers(entt::registry& registry,const GameplayEffectSpec& spec)
	{
		auto& ge_manager = registry.ctx<GameplayEffectManager>();
		auto& ge = ge_manager.LoadGameplayEffect(spec.effect);

		auto& unique_map = registry.ctx<EntityByUniqueIdMap>();
		entt::entity target = unique_map.value.find(spec.target)->second;

		//is modify base
		if (ge.gameplay_effect.duration_policy == GameplayEffect::DurationPolicy::Instant ||
			ge.gameplay_effect.duration_policy == GameplayEffect::DurationPolicy::Duration && ge.period.period > 0)
		{
			for (auto& mod : ge.gameplay_effect.modifiers)
			{
				float value = GetModifyValue(mod.modifier_magnitude);
				auto e = registry.create();
				registry.emplace<ModifierAttribute>(e, target, static_cast<GameplayAttributeType>(mod.attribute), mod.modifier_op, value);
			}
		}
		else
		{
			auto& mod_attr_set = registry.get<InfiniteModifierAttributeSet>(target);
			for (auto& mod : ge.gameplay_effect.modifiers)
			{
				float value = GetModifyValue(mod.modifier_magnitude);
				registry.patch<InfiniteModifierAttributeSet>(target, [&ge,&mod, value](InfiniteModifierAttributeSet& mod_attr_set)
					{
						mod_attr_set.value[mod.attribute].emplace_back(InfiniteModifierAttribute{ ge.name,mod.modifier_op,value });
					});
			}
		}		
	}

	static void RemoveModifiers(entt::registry& registry, const GameplayEffect& ge, entt::entity target)
	{
		if (ge.gameplay_effect.duration_policy == GameplayEffect::DurationPolicy::Infinite)
		{
			registry.patch<InfiniteModifierAttributeSet>(target, [&ge](InfiniteModifierAttributeSet& mod_attr_set)
				{
					for (auto& attrs : mod_attr_set.value)
					{
						auto it = attrs.begin();
						while (it != attrs.end())
						{
							if (it->name == ge.name)
							{
								it = attrs.erase(it);
							}
							else
							{
								++it;
							}
						}
					}
				});
		}
	}

	static void ApplyGameEffectToTarget(entt::registry& registry, entt::entity source, std::string_view effect_name, uint32_t level, entt::entity target)
	{
		auto& apply_ge = registry.ctx_or_set<ApplyGameplayEffect>();
		auto it = apply_ge.value.find(source);
		if (it == apply_ge.value.end())
		{
			apply_ge.value.emplace(source, std::vector<GameplayEffectSpec>());
		}
		apply_ge.value[source].emplace_back(GameplayEffectUtility::MakeGameplayEffectSpec(registry,source,effect_name,level,target));
	}

	static GameplayEffectSpec MakeGameplayEffectSpec(entt::registry& registry,entt::entity source,std::string_view effect,uint32_t level, entt::entity target)
	{
		auto& id = registry.get<UniqueId>(source);
		auto& target_id = registry.get<UniqueId>(target);
		auto& attr_set = registry.get<GameplayAttributeSet>(source);

		GameplayEffectSpec spec(effect, id, target_id, level);
		auto& ge = registry.ctx<GameplayEffectManager>().LoadGameplayEffect(effect.data());
		for (auto& modifier : ge.gameplay_effect.modifiers)
		{
			//todo: snapshot_attrs
		}

		for (auto& execution : ge.gameplay_effect.executions)
		{
			auto execut = GameplayEffectUtility::MakeExecutionCalculation(execution.class_name);
			execut->Capture(registry, spec);
		}

		return spec;
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