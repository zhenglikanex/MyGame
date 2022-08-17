#pragma once

#include "Framework/Game/GAS/GameplayEffectManager.hpp"
#include "Framework/Game/GAS/GameplayEffect.hpp"
#include "Framework/Game/GAS/GameplayEffectExecutionCalculation.hpp"

#include "Framework/Game/Component/Destroy.hpp"
#include "Framework/Game/Component/GAS/ApplyGameplayEffect.hpp"
#include "Framework/Game/Component/GAS/ModifierAttribute.hpp"
#include "Framework/Game/Component/GAS/GameplayEffectSpec.hpp"
#include "Framework/Game/Component/GAS/GameplayState.hpp"

#include "3rdparty/include/entt/entt.hpp"

namespace gameplay_effect_utility
{
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

	static std::vector<entt::entity> GetGameplayEffectSpecEntitiesByOwner(Registry& registry,Remote target)
	{
		std::vector<entt::entity> entities;
		
		auto view = registry.view<GameplayEffectSpec>(entt::exclude<Destroy>);
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

	static void ApplyExecutions(Registry& registry, const GameplayEffectSpec& spec)
	{
		auto& ge_manager = registry.ctx<GameplayEffectManager>();
		auto& ge = ge_manager.LoadGameplayEffect(spec.effect);

		if (ge.gameplay_effect.duration_policy == GameplayEffect::DurationPolicy::Instant ||
			ge.gameplay_effect.duration_policy == GameplayEffect::DurationPolicy::Duration && ge.period.period > 0)
		{
			for (auto& execution : ge.gameplay_effect.executions)
			{
				auto execut = MakeExecutionCalculation(execution.class_name);
				execut->ExecutionCalculation(registry,spec);
			}
		}
	}

	static void ApplyModifiers(Registry& registry,const GameplayEffectSpec& spec)
	{
		auto& ge_manager = registry.ctx<GameplayEffectManager>();
		auto& ge = ge_manager.LoadGameplayEffect(spec.effect);

		//is modify base
		if (ge.gameplay_effect.duration_policy == GameplayEffect::DurationPolicy::Instant ||
			ge.gameplay_effect.duration_policy == GameplayEffect::DurationPolicy::Duration && ge.period.period > 0)
		{
			for (auto& mod : ge.gameplay_effect.modifiers)
			{
				float value = GetModifyValue(mod.modifier_magnitude);

				// Instant和Duration的GE只可以应用GameplayEffect::ModifierOp::Add
				auto e = registry.create();
				registry.emplace<ModifierAttribute>(e, spec.source, spec.target, static_cast<GameplayAttributeType>(mod.attribute), value);
			}
		}
		else
		{
			entt::entity target = registry.get(spec.target);
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

	static std::vector<std::tuple<uint32_t, float>> GetModifiers(Registry& registry, const GameplayEffect& ge)
	{
		std::vector<std::tuple<uint32_t, float>> modifiers;
		modifiers.reserve(ge.gameplay_effect.modifiers.size());

		for (auto& mod : ge.gameplay_effect.modifiers)
		{
			float value = GetModifyValue(mod.modifier_magnitude);
			modifiers.emplace_back(mod.attribute, value);
		}

		return modifiers;
	}

	static std::vector<std::tuple<uint32_t, float>> GetModifiers(Registry& registry,const std::string& name)
	{
		auto& ge_manager = registry.ctx<GameplayEffectManager>();
		auto& ge = ge_manager.LoadGameplayEffect(name);

		return GetModifiers(registry, ge);
	}

	static void RemoveModifiers(Registry& registry, const GameplayEffect& ge, entt::entity target)
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

	static GameplayEffectSpec MakeGameplayEffectSpec(Registry& registry, entt::entity source, std::string_view effect, uint32_t level, entt::entity target)
	{
		auto& id = registry.get<Remote>(source);
		auto& target_id = registry.get<Remote>(target);
		auto& attr_set = registry.get<GameplayAttributeSet>(source);

		auto& ge = registry.ctx<GameplayEffectManager>().LoadGameplayEffect(effect.data());
		GameplayEffectSpec spec(effect, id, target_id, level, ge.period.duration);
		for (auto& modifier : ge.gameplay_effect.modifiers)
		{
			//todo: snapshot_attrs
		}

		for (auto& execution : ge.gameplay_effect.executions)
		{
			auto execut = gameplay_effect_utility::MakeExecutionCalculation(execution.class_name);
			execut->Capture(registry, spec);
		}

		return spec;
	}

	static void ApplyGameEffectToTarget(Registry& registry, entt::entity source, const GameplayEffectSpec& spec)
	{
		auto& apply_ge = registry.ctx_or_set<ApplyGameplayEffect>();
		auto it = apply_ge.value.find(source);
		if (it == apply_ge.value.end())
		{
			apply_ge.value.emplace(source, std::vector<GameplayEffectSpec>());
		}
		apply_ge.value[source].emplace_back(spec);
	}

	static void ApplyGameEffectToTarget(Registry& registry, entt::entity source, std::string_view effect_name, uint32_t level, entt::entity target)
	{
		if (!registry.all_of<Remote>(source))
		{
			return;
		}

		if (effect_name.empty())
		{
			return;
		}

		ApplyGameEffectToTarget(registry,source,MakeGameplayEffectSpec(registry, source, effect_name, level, target));
	}

	static void RemoveGameplayEffectForTarget(Registry& registry,entt::entity target, entt::entity target_ge)
	{
		auto& ge_manager = registry.ctx<GameplayEffectManager>();
		auto& spec = registry.get<GameplayEffectSpec>(target_ge);
		auto& ge = ge_manager.LoadGameplayEffect(spec.effect);
		registry.patch<GameplayTagCountContainer>(target, [&ge](GameplayTagCountContainer& tag_count)
			{
				tag_count.UpdateTagCount(ge.tag.granted_tags, -1);
			});

		// todo:移除技能等
		registry.emplace<Destroy>(target_ge);
	}

	
};