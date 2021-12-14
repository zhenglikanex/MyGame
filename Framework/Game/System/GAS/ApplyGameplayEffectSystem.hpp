#pragma once

#include "Framework/Game/RandomEngine.hpp"

#include "Framework/Game/GAS/GameplayEffectManager.hpp"

#include "Framework/Game/Component/GAS/ApplyGameplayEffect.hpp"
#include "Framework/Game/Component/GAS/GameplayEffectSpec.hpp"
#include "Framework/Game/Component/GAS/GameplayTags.hpp"

#include "Framework/Game/Utility/GameplayEffectUtility.hpp"

#include "Framework/Game/System.hpp"


struct ApplyGameplayEffectSystem : public System
{
	std::vector<std::string> overflow_effects;

	ApplyGameplayEffectSystem(entt::registry& _registry) : System(_registry) { }
	~ApplyGameplayEffectSystem() {}

	bool Initialize() override
	{
		sol::state& lua = registry.ctx<sol::state>();

		return true;
	}

	// 检查目标是否会免疫该ge
	// todo: source_tags来自技能或者拥有者,还没想好怎么实现
	bool HasApplicationImmunity(const std::vector<entt::entity>& entities, const GameplayTags* source_tags)
	{
		return false;
		/*if (!source_tags)
		{
			return false;
		}

		for (auto e : entities)
		{
			if (auto ge_spec = registry.try_get<GameplayEffectSpec>(e))
			{
				if (source_tags->HasAll(ge_spec->ge->immunity.granted_application_immunity_tags.require_tags) ||
					source_tags->HasAny(ge_spec->ge->immunity.granted_application_immunity_tags.ignore_tags))
				{

					return true;
				}
			}
		}

		return false;*/
	}

	// 检查目标是否可以应用GE
	bool CheckApplicationTagRequirements(const GameplayEffect& ge,const GameplayTags* target_tags)
	{
		if (!target_tags && !ge.tag.application_tag_requirements.require_tags.Empty())
		{
			return false;
		}

		if (!target_tags)
		{
			return true;
		}

		return target_tags->tags.HasAll(ge.tag.application_tag_requirements.require_tags) && !target_tags->tags.HasAny(ge.tag.application_tag_requirements.ignore_tags);

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
		
		if (auto apply_ge = registry.try_ctx<ApplyGameplayEffect>())
		{
			for (auto& ge_list : apply_ge->value)
			{
				overflow_effects.clear();

				for (auto& info : ge_list.second)
				{
					auto ge = ge_manager.LoadGameplayEffect(info.effect);

					auto target_tags = registry.try_get<GameplayTags>(info.target);
					auto source_tags = registry.try_get<GameplayTags>(info.source);

					auto& target_spec_entities =  GameplayEffectUtility::GetGameplayEffectSpecEntitiesByOwner(registry, info.target);

					if (HasApplicationImmunity(target_spec_entities, source_tags))
					{
						continue;
					}

					if (!CheckApplicationTagRequirements(*ge,target_tags))
					{
						std::cout << "CheckApplicationTagRequirements Failed! " << std::endl;
						continue;
					}

					if (!CheckApplicationChance(*ge))
					{
						continue;
					}

					GameplayEffectSpec spec(ge,info.source, info.target, info.level);

					ApplyModifiers(spec);

					if (spec.ge->gameplay_effect.duration_policy == GameplayEffect::DurationPolicy::Duration ||
						spec.ge->gameplay_effect.duration_policy == GameplayEffect::DurationPolicy::Infinite)
					{
						ApplyGameEffectToTarget(spec, target_spec_entities, info.target);
					}

					// todo:
					if (overflow_effects.size() > 0)
					{

					}
				}
				
			}

			apply_ge->value.clear();
		}		
	}

	void ApplyModifiers(const GameplayEffectSpec& spec)
	{
		GameplayEffectUtility::ApplyModifiers(registry, spec);
	}

	void ApplyGameEffectToTarget(const GameplayEffectSpec& spec,const std::vector<entt::entity>& target_spec_entities,entt::entity target)
	{		
		bool stacking = false;

		if (spec.ge->stacking.stacking_type != GameplayEffect::StackingType::None)
		{
			for (auto e : target_spec_entities)
			{
				stacking = true;

				auto& act_spec = registry.get<GameplayEffectSpec>(e);
				if (&spec.ge == &act_spec.ge)
				{
					// 处理叠加相关
					bool reset_duration = false;
					bool reset_period = false;
					if (act_spec.stack_count >= spec.ge->stacking.stack_limit_count)
					{
						for (auto& ge_name : spec.ge->overflow.overflow_effects)
						{
							overflow_effects.push_back(ge_name);
						}

						if (spec.ge->overflow.deny_overflow_application && spec.ge->overflow.clear_stack_on_overflow)
						{
							act_spec.stack_count = 0;
						}

						if (spec.ge->stacking.stack_duration_refresh_policy &&
							!spec.ge->overflow.deny_overflow_application)
						{
							reset_duration = true;
						}

						if (spec.ge->stacking.statck_period_reset_policy &&
							!spec.ge->overflow.deny_overflow_application)
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
				}
				break;
			}
		}

		if (!stacking)
		{
			auto e = registry.create();
			registry.emplace<GameplayEffectSpec>(e, spec);

			if (!spec.ge->tag.granted_tags.Empty())
			{
				auto& tags = registry.get_or_emplace<GameplayTags>(target);
				tags.attacth_tags.emplace(e, spec.ge->tag.granted_tags);

				tags.tags.Clear();

				std::unordered_map<GameplayTag, bool> flag;				
				for (auto& item : tags.attacth_tags)
				{
					for (auto it = item.second.Begin();it != item.second.End(); ++it)
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
		
		// todo 赋予技能
		// todo 添加cues
	}

	void RemoveGameplayEffectsWithTags(const GameplayEffectSpec& spec,const std::vector<entt::entity>& entities)
	{
		for (auto e : entities)
		{
			auto& act_sepc = registry.get<GameplayEffectSpec>(e);
			if (act_sepc.ge->tag.remove_gameplay_effects_with_tags.HasAny(spec.ge->tag.gameplay_effect_asset_tags))
			{
				auto tags = registry.try_get<GameplayTags>(act_sepc.owner);
				if (tags)
				{
					tags->attacth_tags.erase(e);
				}

				registry.destroy(e);
			}
		}
	}

	void Finalize() override
	{
		
	}
};
