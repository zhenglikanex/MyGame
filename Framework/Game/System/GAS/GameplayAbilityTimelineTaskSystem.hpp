#pragma once

#include "Framework/Game/GAS/GameplayAbilityManager.hpp"

#include "Framework/Game/Component/AnimationClip.hpp"
#include "Framework/Game/GAS/GameplayTag.hpp"
#include "Framework/Game/GAS/GameplayAbilityTimeline.hpp"
#include "Framework/Game/GAS/GameplayAbilityTimelineManager.hpp"

#include "Framework/Game/Component/GAS/GameplayAbilitySpec.hpp"
#include "Framework/Game/Component/GAS/GameplayHitTestTask.hpp"
#include "Framework/Game/Component/GAS/GameplayAbilityTimelineTask.hpp"
#include "Framework/Game/Component/GAS/GameplayAbilityScript.hpp"

#include "Framework/Game/System.hpp"

struct GameplayAbilityTimelineTaskSystem : public System
{
	GameplayAbilityTimelineTaskSystem(Registry& _registry) : System(_registry) { }

	void UpdateGameplayAbilityTimelineSpec(entt::registry& reg, entt::entity e)
	{
		// 删除进行中的tag
		auto& ability = registry.get<GameplayAbilitySpec>(e);
		
		entt::entity source = registry.get(ability.source);

		if(source == entt::null)
		{
			return;
		}

		auto& spec = registry.get<GameplayAbilityTimelineTask>(e);
		auto& manager = registry.ctx<GameplayAbilityTimelineManager>();
		auto& timeline = manager.LoadGameplayAbilityTimeline(spec.name);

		std::vector<GameplayAbilityTimeline::Event> events = timeline.GetEvent(spec.frame);
		for(auto& event : events)
		{
			if (event.type == timeline_event::EventType::kTag 
				&& event.status == GameplayAbilityTimeline::EventStatus::kDoing)
			{
				const timeline_event::TagEvent& tag_event = timeline.GetTagEventByIndex(event.index);
				registry.patch<GameplayTagCountContainer>(source, [&tag_event](GameplayTagCountContainer& tag_count)
					{
						tag_count.UpdateTagCount(tag_event.granted_tags, -1);
					});
			}
		}
	}

	bool Initialize() override
	{
		registry.on_update<GameplayAbilityTimelineTask>()
			.connect<&GameplayAbilityTimelineTaskSystem::UpdateGameplayAbilityTimelineSpec>(this);

		return true;
	}

	void FixedUpdate(float dt) override
	{
		using namespace timeline_event;

		auto& manager = registry.ctx<GameplayAbilityTimelineManager>();

		auto view = registry.view<GameplayAbilitySpec, GameplayAbilityScript,GameplayAbilityTimelineTask>();
		for(auto e : view)
		{
			auto&[spec,script,task] = view.get<GameplayAbilitySpec,GameplayAbilityScript,GameplayAbilityTimelineTask>(e);

			if(task.is_done)
			{
				continue;
			}

			entt::entity source = registry.get(spec.source);
			if(source == entt::null)
			{
				continue;
			}

			const GameplayAbilityTimeline& timeline = manager.LoadGameplayAbilityTimeline(task.name);
			std::vector<GameplayAbilityTimeline::Event> events = timeline.GetEvent(task.frame);

			for(auto& event : events)
			{
				switch (event.type)
				{
				case EventType::kAnimation:
					ProcessAnimationEvent(source, timeline, event);
					break;
				case EventType::kCollider:
					ProcessColliderEvent(source,e, task, timeline, event);
					break;

				case EventType::kTag:
					ProcessTagEvent(source,timeline, event);
				}
			}

			++task.frame;

			if (task.frame >= timeline.frame_count())
			{
				task.is_done = true;
				script.value[task.completed](script.value);
			}
		}
	}

	void ProcessAnimationEvent(entt::entity e, const GameplayAbilityTimeline& timeline, const GameplayAbilityTimeline::Event& event) const
	{
		auto& animation_event = timeline.GetAnimationEventByIndex(event.index);
		registry.replace<AnimationClip>(e, animation_event.name);
		
	}

	void ProcessColliderEvent(entt::entity e,entt::entity ability, GameplayAbilityTimelineTask& spec,const GameplayAbilityTimeline& timeline, const GameplayAbilityTimeline::Event& event) const
	{
		auto& collider_event = timeline.GetColliderEventByIndex(event.index);
		if(event.status == GameplayAbilityTimeline::EventStatus::kStart 
			|| (event.status == GameplayAbilityTimeline::EventStatus::kDoing && !collider_event.once))
		{
			mat4 transform;
			if(collider_event.follow)
			{
				transform = GetMatrix4x4(registry.get<Transform>(e)) * collider_event.transform;
			}

			if(!collider_event.follow)
			{
				// 记录下开始时的位置
				if (event.status == GameplayAbilityTimeline::EventStatus::kStart)
				{
					if (event.index > spec.collider_event_start_transforms.size())
					{
						spec.collider_event_start_transforms.resize(event.index);
					}
					spec.collider_event_start_transforms[event.index] = GetMatrix4x4(registry.get<Transform>(e)) * collider_event.transform;
				}

				transform = spec.collider_event_start_transforms[event.index];
			}

			auto& task = registry.get_or_emplace<GameplayHitTestTask>(ability);
			task.value.emplace_back(GameplayHitType::kOverlap, collider_event.geometry, transform, "on_hit");
		}

	}

	void ProcessTagEvent(entt::entity e, const GameplayAbilityTimeline& timeline, const GameplayAbilityTimeline::Event& event) const
	{
		auto tag_event = timeline.GetTagEventByIndex(event.index);
		switch (event.status)
		{
		case GameplayAbilityTimeline::EventStatus::kStart:
			registry.patch<GameplayTagCountContainer>(e, [&tag_event](GameplayTagCountContainer& tag_count)
				{
					tag_count.UpdateTagCount(tag_event.granted_tags, 1);
				});
			
			break;
		case GameplayAbilityTimeline::EventStatus::kDoing: 
			break;
		case GameplayAbilityTimeline::EventStatus::kEnd:
			registry.patch<GameplayTagCountContainer>(e, [&tag_event](GameplayTagCountContainer& tag_count)
				{
					tag_count.UpdateTagCount(tag_event.granted_tags, -1);
				});

			break;
		}
	}


	void Finalize() override
	{

	}
};
