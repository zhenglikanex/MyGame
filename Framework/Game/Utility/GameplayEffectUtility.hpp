#pragma once

#include "Framework/Game/GAS/GameplayEffectManager.hpp"
#include "Framework/Game/GAS/GameplayEffect.hpp"
#include "Framework/Game/GAS/GameplayEffectExecutionCalculation.hpp"

#include "Framework/Game/Component/EntityByUniqueIdMap.hpp"
#include "Framework/Game/Component/Destory.hpp"
#include "Framework/Game/Component/GAS/ApplyGameplayEffect.hpp"
#include "Framework/Game/Component/GAS/ModifierAttribute.hpp"
#include "Framework/Game/Component/GAS/GameplayEffectSpec.hpp"
#include "Framework/Game/Component/GAS/GameplayTags.hpp"

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

	static std::vector<entt::entity> GetGameplayEffectSpecEntitiesByOwner(Registry& registry,UniqueId target)
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

	static void ApplyExecutions(Registry& registry, const GameplayEffectSpec& spec)
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

	static std::vector<std::tuple<uint32_t, float>> GetModifiers(Registry& registry,const std::string& name)
	{
		auto& ge_manager = registry.ctx<GameplayEffectManager>();
		auto& ge = ge_manager.LoadGameplayEffect(name);

		return GetModifiers(registry, ge);
	}

	static std::vector<std::tuple<uint32_t,float>> GetModifiers(Registry& registry, const GameplayEffect& ge)
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

	static void UpdateGameplayTag(Registry& registry, entt::entity target)
	{
		if (!registry.all_of<UniqueId>(target))
		{
			return;
		}

		auto& ge_manager = registry.ctx<GameplayEffectManager>();

		auto& tags = registry.get<GameplayTags>(target);
		tags.tags.Clear();

		auto& target_spec_entities = GameplayEffectUtility::GetGameplayEffectSpecEntitiesByOwner(registry, registry.get<UniqueId>(target));

		std::unordered_map<GameplayTag, bool> flag;
		for (auto e : target_spec_entities)
		{
			if (!registry.all_of<Destory>(e))
			{
				auto& spec = registry.get<GameplayEffectSpec>(e);
				auto& ge = ge_manager.LoadGameplayEffect(spec.effect);
				for (auto it = ge.tag.granted_tags.Begin(); it != ge.tag.granted_tags.End(); ++it)
				{
					auto& tag = *it;
					if (flag.find(tag) == flag.end())
					{
						flag[tag] = true;
						tags.tags.Push(tag);
					}
				}

				for (auto it = spec.dynamic_granted_tags.Begin(); it != spec.dynamic_granted_tags.End(); ++it)
				{
					auto& tag = *it;
					if (flag.find(tag) == flag.end())
					{
						flag[tag] = true;
						tags.tags.Push(tag);
					}
				}
			}
		}
	}

	static void ApplyGameEffectToTarget(Registry& registry, entt::entity source, std::string_view effect_name, uint32_t level, entt::entity target)
	{
		if (!registry.all_of<UniqueId>(source))
		{
			return;
		}

		if (effect_name.size() <= 0)
		{
			return;
		}

		GameplayEffectUtility::ApplyGameEffectToTarget(registry,source,GameplayEffectUtility::MakeGameplayEffectSpec(registry, source, effect_name, level, target));
	}

	static void ApplyGameEffectToTarget(Registry& registry,entt::entity source,const GameplayEffectSpec& spec)
	{
		auto& apply_ge = registry.ctx_or_set<ApplyGameplayEffect>();
		auto it = apply_ge.value.find(source);
		if (it == apply_ge.value.end())
		{
			apply_ge.value.emplace(source, std::vector<GameplayEffectSpec>());
		}
		apply_ge.value[source].emplace_back(spec);
	}

	static GameplayEffectSpec MakeGameplayEffectSpec(Registry& registry,entt::entity source,std::string_view effect,uint32_t level, entt::entity target)
	{
		auto& id = registry.get<UniqueId>(source);
		auto& target_id = registry.get<UniqueId>(target);
		auto& attr_set = registry.get<GameplayAttributeSet>(source);

		auto& ge = registry.ctx<GameplayEffectManager>().LoadGameplayEffect(effect.data());
		GameplayEffectSpec spec(effect, id, target_id, level,ge.period.duration);
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