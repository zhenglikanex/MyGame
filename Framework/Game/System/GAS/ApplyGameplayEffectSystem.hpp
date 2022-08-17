#pragma once

#include "Framework/Game/RandomEngine.hpp"

#include "Framework/Game/GAS/GameplayEffectManager.hpp"

#include "Framework/Game/Component/Destroy.hpp"
#include "Framework/Game/Component/GAS/ApplyGameplayEffect.hpp"
#include "Framework/Game/Component/GAS/GameplayEffectSpec.hpp"
#include "Framework/Game/Component/GAS/GameplayState.hpp"

#include "Framework/Game/Utility/GameplayEffectUtility.hpp"

#include "Framework/Game/System.hpp"


struct ApplyGameplayEffectSystem : public System
{
	std::vector<std::string> overflow_effects;

	ApplyGameplayEffectSystem(Registry& _registry) : System(_registry) { }

	bool Initialize() override
	{
		return true;
	}

	// 检查目标是否会免疫该ge
	// todo: source_tags来自技能或者拥有者,还没想好怎么实现
	bool HasApplicationImmunity(const std::vector<entt::entity>& entities,const GameplayTagCountContainer& tag_count)
	{
		return false;
	}

	// 检查目标是否可以应用GE
	bool CheckApplicationTagRequirements(const GameplayEffect& ge,const GameplayTagCountContainer& tag_count)
	{
		return tag_count.HasAll(ge.tag.application_tag_requirements.require_tags) && !tag_count.HasAny(ge.tag.application_tag_requirements.ignore_tags);
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

	void FixedUpdate(float dt) override
	{
		GameplayEffectManager& ge_manager = registry.ctx<GameplayEffectManager>();
		
		if (auto apply_ge = registry.try_ctx<ApplyGameplayEffect>())
		{
			for (auto& ge_list : apply_ge->value)
			{
				overflow_effects.clear();

				for (auto& spec : ge_list.second)
				{
					entt::entity target = registry.get(spec.target);
					entt::entity source = registry.get(spec.source);

					if (target == entt::null || source == entt::null)
					{
						continue;
					}

					if (!(registry.all_of<GameplayState>(target) && registry.all_of<GameplayState>(source)))
					{
						continue;
					}

					auto& ge = ge_manager.LoadGameplayEffect(spec.effect);

					auto& target_tag_count = registry.get<GameplayTagCountContainer>(target);
					auto& source_tag_count = registry.get<GameplayTagCountContainer>(source);

					auto& target_spec_entities = gameplay_effect_utility::GetGameplayEffectSpecEntitiesByOwner(registry,spec.target);

					if (HasApplicationImmunity(target_spec_entities, source_tag_count))
					{
						continue;
					}

					if (!CheckApplicationTagRequirements(ge, target_tag_count))
					{
						continue;
					}

					if (!CheckApplicationChance(ge))
					{
						continue;
					}
					
					gameplay_effect_utility::ApplyExecutions(registry, spec);
					gameplay_effect_utility::ApplyModifiers(registry, spec);

					if (ge.gameplay_effect.duration_policy == GameplayEffect::DurationPolicy::Duration ||
						ge.gameplay_effect.duration_policy == GameplayEffect::DurationPolicy::Infinite)
					{
						ApplyGameEffectToTarget(target,spec, ge, target_spec_entities);
					}

					RemoveGameplayEffectsWithTags(target,ge,target_spec_entities);
					
					// todo:
					if (overflow_effects.size() > 0)
					{

					}
				}
			}

			apply_ge->value.clear();
		}		
	}

	void ApplyGameEffectToTarget(entt::entity target,GameplayEffectSpec& spec,const GameplayEffect& ge,std::vector<entt::entity>& target_spec_entities)
	{		
		bool apply_instance = false;
		bool stacking = false;

		if (ge.stacking.stacking_type != GameplayEffect::StackingType::None)
		{
			for (auto e : target_spec_entities)
			{
				auto& act_spec = registry.get<GameplayEffectSpec>(e);
				if (ge.name == act_spec.effect)
				{
					stacking = true;

					// 处理叠加相关
					bool reset_duration = false;
					bool reset_period = false;
					if (act_spec.stack_count >= ge.stacking.stack_limit_count)
					{
						for (auto& effect : ge.overflow.overflow_effects)
						{
							overflow_effects.push_back(effect);
						}

						if (ge.overflow.deny_overflow_application && ge.overflow.clear_stack_on_overflow)
						{
							act_spec.stack_count = 0;
						}

						if (ge.stacking.stack_duration_refresh_policy &&
							!ge.overflow.deny_overflow_application)
						{
							reset_duration = true;
						}

						if (ge.stacking.statck_period_reset_policy &&
							!ge.overflow.deny_overflow_application)
						{
							reset_period = true;
						}
					}
					else
					{
						++act_spec.stack_count;
						reset_duration = true;
						reset_period = true;
					}

					if (reset_duration)
					{
						act_spec.duration = 0;
					}

					if (reset_period)
					{
						act_spec.period = 0;
					}

					break;
				}
			}
		}

		if (!stacking)
		{
			registry.patch<GameplayTagCountContainer>(target, [&ge,&spec](GameplayTagCountContainer& tag_count)
				{
					tag_count.UpdateTagCount(ge.tag.granted_tags,1);
					tag_count.UpdateTagCount(spec.dynamic_granted_tags,1);
				});
			
			auto e = registry.create_predict(registry.get(spec.target));
			registry.emplace<GameplayEffectSpec>(e,std::move(spec));
			target_spec_entities.push_back(e);

			// todo 赋予技能
		}

		//添加cue
	}

	void RemoveGameplayEffectsWithTags(entt::entity target,const GameplayEffect& ge,const std::vector<entt::entity>& target_spec_entities)
	{
		auto& ge_manager = registry.ctx<GameplayEffectManager>();

		for (auto e : target_spec_entities)
		{
			auto& act_sepc = registry.get<GameplayEffectSpec>(e);
			auto& act_ge = ge_manager.LoadGameplayEffect(act_sepc.effect);
			if (act_ge.tag.gameplay_effect_asset_tags.HasAny(ge.tag.remove_gameplay_effects_with_tags))
			{
				gameplay_effect_utility::RemoveGameplayEffectForTarget(registry,target,e);
			}
		}
	}

	void Finalize() override
	{
		
	}
};
