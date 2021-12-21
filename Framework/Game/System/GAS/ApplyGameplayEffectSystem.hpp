#pragma once

#include "Framework/Game/RandomEngine.hpp"
#include "Framework/Game/GameHelper.hpp"

#include "Framework/Game/GAS/GameplayEffectManager.hpp"

#include "Framework/Game/Component/EntityByUniqueIdMap.hpp"
#include "Framework/Game/Component/Destory.hpp"
#include "Framework/Game/Component/GAS/ApplyGameplayEffect.hpp"
#include "Framework/Game/Component/GAS/GameplayEffectSpec.hpp"
#include "Framework/Game/Component/GAS/GameplayTags.hpp"
#include "Framework/Game/Component/GAS/GameplayState.hpp"

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
		GameHelper& game_helper = registry.ctx<GameHelper>();
		
		if (auto apply_ge = registry.try_ctx<ApplyGameplayEffect>())
		{
			for (auto& ge_list : apply_ge->value)
			{
				overflow_effects.clear();

				for (auto& info : ge_list.second)
				{
					if (!(registry.has<UniqueId>(info.target) && registry.has<UniqueId>(info.source) &&
						registry.has<GameplayState>(info.target) && registry.has<GameplayState>(info.source)))
					{
						continue;
					}

					auto& ge = ge_manager.LoadGameplayEffect(info.effect);

					auto target_tags = registry.try_get<GameplayTags>(info.target);
					auto source_tags = registry.try_get<GameplayTags>(info.source);

					auto& target_spec_entities = GameplayEffectUtility::GetGameplayEffectSpecEntitiesByOwner(registry,registry.get<UniqueId>(info.target));

					if (HasApplicationImmunity(target_spec_entities, source_tags))
					{
						continue;
					}

					if (!CheckApplicationTagRequirements(ge,target_tags))
					{
						std::cout << "CheckApplicationTagRequirements Failed! " << std::endl;
						continue;
					}

					if (!CheckApplicationChance(ge))
					{
						continue;
					}
					
					ApplyModifiers(ge,info.target,info.level);

					bool need_update_gameplay_tags = false;
					if (ge.gameplay_effect.duration_policy == GameplayEffect::DurationPolicy::Duration ||
						ge.gameplay_effect.duration_policy == GameplayEffect::DurationPolicy::Infinite)
					{
						need_update_gameplay_tags = ApplyGameEffectToTarget(info, ge, target_spec_entities);
					}
					need_update_gameplay_tags = need_update_gameplay_tags && RemoveGameplayEffectsWithTags(ge, target_spec_entities);
					if (need_update_gameplay_tags)
					{
						UpdateTargetGameplayTag(info.target,target_spec_entities);
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

	void ApplyModifiers(const GameplayEffect& ge,entt::entity target,uint32_t level)
	{
		GameplayEffectUtility::ApplyModifiers(registry, ge,target,level);
	}

	bool ApplyGameEffectToTarget(const GameplayEffectInfo& info,const GameplayEffect& ge,std::vector<entt::entity>& target_spec_entities)
	{		
		bool need_update_gameply_tag = false;
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
			auto e = registry.create();
			registry.emplace<GameplayEffectSpec>(e, info.effect, registry.get<UniqueId>(info.source), registry.get<UniqueId>(info.target), info.level);
			target_spec_entities.push_back(e);
			
			need_update_gameply_tag = !ge.tag.granted_tags.Empty();

			// todo 赋予技能
		}

		//添加cue

		return need_update_gameply_tag;
	}

	bool RemoveGameplayEffectsWithTags(const GameplayEffect& ge,const std::vector<entt::entity>& target_spec_entities)
	{
		bool need_update_gameply_tag = false;
		auto& ge_manager = registry.ctx<GameplayEffectManager>();

		for (auto e : target_spec_entities)
		{
			auto& act_sepc = registry.get<GameplayEffectSpec>(e);
			auto& act_ge = ge_manager.LoadGameplayEffect(act_sepc.effect);
			if (act_ge.tag.gameplay_effect_asset_tags.HasAny(ge.tag.remove_gameplay_effects_with_tags))
			{
				need_update_gameply_tag = true;
				registry.emplace<Destory>(e);
			}
		}

		return need_update_gameply_tag;
	}

	void UpdateTargetGameplayTag(entt::entity target,const std::vector<entt::entity>& target_spec_entities)
	{
		auto& ge_manager = registry.ctx<GameplayEffectManager>();

		auto& tags = registry.get_or_emplace<GameplayTags>(target);
		tags.tags.Clear();

		std::unordered_map<GameplayTag, bool> flag;
		for (auto e : target_spec_entities)
		{
			if (!registry.has<Destory>(e))
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
			}
		}
	}

	void Finalize() override
	{
		
	}
};
