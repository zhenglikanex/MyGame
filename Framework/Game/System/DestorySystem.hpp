#pragma once

#include "Framework/Game/Component/Destroy.hpp"
#include "Framework/Game/Component/GAS/GameplayAbilitySpec.hpp"
#include "Framework/Game/Component/GAS/GameplayEffectSpec.hpp"

#include "Framework/Game/System.hpp"
#include "Framework/Game/Component/GAS/GameplayAbilityTimelineTask.hpp"
#include "Framework/Game/GAS/GameplayAbilityTimelineManager.hpp"


struct DestorySystem : System
{
public:
	explicit DestorySystem(Registry& _registry) : System(_registry)
	{
	}

	bool Initialize() override
	{
		return true;
	}

	void FixedUpdate(float dt) override
	{
		// 缓存下当帧销毁的entity,将在销毁中被销毁的关联对象移到下一帧销毁
		auto view = registry.view<Destroy>();
		std::vector<entt::entity> entities;
		entities.reserve(view.size());
		for (auto e : view)
		{
			entities.push_back(e);
		}

		DestroyGameplayAbility();
		DestroyGameplayAbilityTimeline();
		DestroyGameplayEffect();
		DestroyEntity(entities);
	}

	void DestroyGameplayAbility()
	{
		auto& ga_manager = registry.ctx<GameplayAbilityManager>();
		auto view = registry.view<GameplayAbilitySpec,Destroy>();
		for (auto e : view)
		{
			auto& spec = view.get<GameplayAbilitySpec>(e);
			auto& ga = ga_manager.LoadGameplayAbility(spec.ability);

			INFO("Destory Ability {}", spec.ability);

			auto source = registry.get(spec.source);
			if (source != entt::null)
			{
				auto& game_state = registry.ctx<GameState>();
				INFO("frame {} DestoryGameplayAbility {}", game_state.run_frame,e);

				registry.patch<GameplayTagCountContainer>(source,[&ga](GameplayTagCountContainer& tag_count)
					{
						tag_count.UpdateTagCount(ga.activation_owned_tags, -1);
					});
			}

			for (auto& item : spec.entity_manager)
			{
				auto e = registry.get(item.second);
				if (e != entt::null)
				{
					registry.emplace<Destroy>(e);
				}
			}
		}
	}

	void DestroyGameplayAbilityTimeline()
	{
		auto view = registry.view<GameplayAbilitySpec, GameplayAbilityTimelineTask, Destroy>();

		for (auto e : view)
		{
			// 删除进行中的tag
			auto& ability = view.get<GameplayAbilitySpec>(e);

			entt::entity source = registry.get(ability.source);

			if (source == entt::null)
			{
				return;
			}

			auto& spec = view.get<GameplayAbilityTimelineTask>(e);
			auto& manager = registry.ctx<GameplayAbilityTimelineManager>();
			auto& timeline = manager.LoadGameplayAbilityTimeline(spec.name);

			std::vector<GameplayAbilityTimeline::Event> events = timeline.GetEvent(spec.frame);
			for (auto& event : events)
			{
				if (event.type == timeline_event::EventType::kTag
					&& (event.status == GameplayAbilityTimeline::EventStatus::kDoing 
						|| event.status == GameplayAbilityTimeline::EventStatus::kEnd))
				{
					const timeline_event::TagEvent& tag_event = timeline.GetTagEventByIndex(event.index);
					registry.patch<GameplayTagCountContainer>(source, [&tag_event](GameplayTagCountContainer& tag_count)
						{
							for(auto it = tag_event.granted_tags.Begin();it != tag_event.granted_tags.End();++it)
							{
								INFO("Remove Tag Value{} Mask{}", it->value(), it->mask());
							}
							tag_count.UpdateTagCount(tag_event.granted_tags, -1);
						});
				}
			}
		}
	}
	void DestroyGameplayEffect()
	{
		auto& ge_manager = registry.ctx<GameplayEffectManager>();
		auto view = registry.view<GameplayEffectSpec, Destroy>();
		for(auto e : view)
		{
			auto& spec = view.get<GameplayEffectSpec>(e);

			auto& ge =  ge_manager.LoadGameplayEffect(spec.effect);
			auto target = registry.get(spec.target);
			if (target != entt::null)
			{
				auto& ge = ge_manager.LoadGameplayEffect(spec.effect);
				registry.patch<GameplayTagCountContainer>(target, [&ge](GameplayTagCountContainer& tag_count)
				{
					tag_count.UpdateTagCount(ge.tag.granted_tags, -1);
				});

				// todo:移除技能等
				registry.emplace<Destroy>(e);
			}
		}
	}

	void DestroyEntity(const std::vector<entt::entity>& entities)
	{
		if(!entities.empty())
		{
			auto& game_state = registry.ctx<GameState>();
			INFO("frame {} DestoryEntity size{}", entities.size(), game_state.run_frame);
		}
		
		registry.destroy(entities.begin(), entities.end());
	}

	void Finalize() override
	{

	}
};
