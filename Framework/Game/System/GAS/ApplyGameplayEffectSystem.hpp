#pragma once

#include "Framework/Game/RandomEngine.hpp"

#include "Framework/Game/GAS/GameplayEffectManager.hpp"

#include "Framework/Game/Component/GAS/ApplyGameplayEffectList.hpp"
#include "Framework/Game/Component/GAS/GameplayEffectList.hpp"
#include "Framework/Game/Component/GAS/GameplayTags.hpp"

#include "Framework/Game/System.hpp"

struct ApplyGameplayEffectSystem : public System
{
	std::vector<std::string> overflow_effects;

	entt::observer apply_ge_list{ registry,entt::collector.group<GameplayEffectList,GameplayTags,ApplyGameplayEffectList>()
		.update<ApplyGameplayEffectList>().where<GameplayEffectList>() };

	ApplyGameplayEffectSystem(entt::registry& _registry) : System(_registry) { }
	~ApplyGameplayEffectSystem() {}

	bool Initialize() override
	{
		sol::state& lua = registry.ctx<sol::state>();

		return true;
	}

	// 是否有激活的ge免疫当前的ge
	// todo: source_tags来自技能或者拥有者,还没想好怎么实现
	bool HasApplicationImmunity(const GameplayEffectList& target_activity_ges, const GameplayTags& source_tags)
	{
		for (auto& activity_ge : target_activity_ges.value)
		{
			return source_tags.HasAll(activity_ge.gameplay_effect.immunity.granted_application_immunity_tags.require_tags) ||
				source_tags.HasAny(activity_ge.gameplay_effect.immunity.granted_application_immunity_tags.ignore_tags);
		}

		return false;
	}

	// 检查是否可以应用GE
	bool CheckApplicationTagRequirements(const GameplayEffect& ge, const GameplayTags& source_tags, const GameplayTags& target_tags)
	{
		if (!source_tags.HasAll(ge.tag.application_tag_requirements.require_tags))
		{
			return false;
		}

		if (target_tags.HasAny(ge.tag.application_tag_requirements.ignore_tags))
		{
			return false;
		}

		return true;
	}

	// 检查概率
	bool CheckApplicationChance(const GameplayEffect& ge)
	{
		if (ge.application.chance_to_apply_to_target == 0)
		{
			return false;
		}

		if (ge.application.chance_to_apply_to_target == 100)
		{
			return true;
		}

		auto& random_engine = registry.ctx<RandomEngine>();
		uint32_t value = random_engine.Random(0, 100);

		return ge.application.chance_to_apply_to_target >= value;
	}


	void FixedUpdate(fixed16 dt) override
	{
		sol::state& lua = registry.ctx<sol::state>();
		GameplayEffectManager& ge_manager = registry.ctx<GameplayEffectManager>();

		auto view = registry.view<ApplyGameplayEffectList, GameplayEffectList, GameplayTags>();
		if (apply_ge_list.size() > 0)
		{
			for (auto e : apply_ge_list)
			{
				overflow_effects.clear();

				auto&[apply_ge_list, ge_list, tags] = view.get<ApplyGameplayEffectList, GameplayEffectList, GameplayTags>(e);
				for (auto& ge_info : apply_ge_list.value)
				{
					auto& source_tags = registry.get<GameplayTags>(ge_info.source);
					GameplayEffectSpec spec(ge_manager.LoadGameplayEffect(ge_info.name), ge_info.level);

					if (HasApplicationImmunity(ge_list, source_tags))
					{
						break;
					}

					if (!CheckApplicationTagRequirements(spec.gameplay_effect, source_tags, tags))
					{
						break;
					}

					if (!CheckApplicationChance(spec.gameplay_effect))
					{
						break;
					}

					ApplyModifiers(spec,e);
					
					if (spec.gameplay_effect.gameplay_effect.duration_policy == GameplayEffect::DurationPolicy::Duration ||
						spec.gameplay_effect.gameplay_effect.duration_policy == GameplayEffect::DurationPolicy::Infinite)
					{
						ApplyGameEffectToTarget(spec, ge_list,tags);
					}
				}

				// todo:
				if (overflow_effects.size() > 0)
				{
					
				}
			}

			apply_ge_list.clear();

			// todo:处理overflow_effects
		}
	}

	void ApplyModifiers(const GameplayEffectSpec& spec,entt::entity target)
	{

	}

	void ApplyGameEffectToTarget(const GameplayEffectSpec& spec,GameplayEffectList& ge_list,GameplayTags& tags)
	{
		bool can_granted_tags = false;
		
		if (spec.gameplay_effect.stacking.stacking_type == GameplayEffect::StackingType::None)
		{
			ge_list.value.push_back(spec);
			can_granted_tags = true;
		}
		else
		{
			auto it = std::find_if(ge_list.value.begin(), ge_list.value.end(), [&spec](const GameplayEffectSpec& value)
			{
				return &value.gameplay_effect == &spec.gameplay_effect;
			});

			if (it == ge_list.value.end())
			{
				ge_list.value.push_back(spec);
				can_granted_tags = true;
			}

			// 处理叠加相关
			bool reset_duration = false;
			bool reset_period = false;
			if (it->stack_count >= spec.gameplay_effect.stacking.stack_limit_count)
			{
				for (auto& ge_name : spec.gameplay_effect.overflow.overflow_effects)
				{
					overflow_effects.push_back(ge_name);
				}

				if (spec.gameplay_effect.overflow.deny_overflow_application && spec.gameplay_effect.overflow.clear_stack_on_overflow)
				{
					it->stack_count = 0;
				}

				if (spec.gameplay_effect.stacking.stack_duration_refresh_policy && 
					!spec.gameplay_effect.overflow.deny_overflow_application)
				{
					reset_duration = true;
				}

				if (spec.gameplay_effect.stacking.statck_period_reset_policy &&
					!spec.gameplay_effect.overflow.deny_overflow_application)
				{
					reset_period = true;
				}
			}
			else
			{
				++it->stack_count;
				reset_duration = true;
				reset_period = true;
			}

			if (reset_duration)
			{
				it->duration = 0;
			}

			if (reset_period)
			{
				it->period = 0;
			}
		}

		if (can_granted_tags)
		{
			tags.value.emplace(&spec.gameplay_effect, spec.gameplay_effect.tag.granted_tags);
		}

		// todo 赋予技能
		// todo 添加cues
	}

	void RemoveGameplayEffectsWithTags(const GameplayEffectSpec& spec,GameplayEffectList& ge_list)
	{
		for (auto& activity_ge : ge_list.value)
		{
			if (spec.gameplay_effect.tag.remove_gameplay_effects_with_tags.HasAny(activity_ge.gameplay_effect.tag.gameplay_effect_asset_tags) ||
				spec.gameplay_effect.tag.remove_gameplay_effects_with_tags.HasAny(activity_ge.gameplay_effect.tag.granted_tags))
			{
				//todo
			}
		}

	}

	void Finalize() override
	{
		
	}
};
