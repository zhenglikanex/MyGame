#pragma once

#include <functional>

#include "Framework/Game/Component/SkillState.hpp"
#include "Framework/Game/Component/AnimationClip.hpp"
#include "Framework/Game/Component/Skill.hpp"
#include "Framework/Game/Component/Weapon.hpp"
#include "Framework/Game/Component/Transform.hpp"

#include "Framework/Game/System.hpp"

struct SkillStateSystem : System
{
	std::unordered_map<std::string, std::function<void(entt::entity, const SkillGraph&, const SkillState&, SkillParams&)>> events;

	SkillStateSystem(entt::registry& registry) : System(registry)
	{
		events.emplace("OnResetSkillParam", [this](entt::entity e, const SkillGraph& skill_graph, const SkillState& skill_state,SkillParams& skill_params)
		{
			skill_params.value.find("skill")->second.int_value = 0;
		});

		events.emplace("OnLaunchSkill", [this](entt::entity e, const SkillGraph& skill_graph, const SkillState& skill_state, SkillParams& skill_params)
		{
			// todo:读取单个技能配置根据技能配置创建技能
			SkillType type = SkillType::kNotLockAnim;
			if (type == SkillType::kNotLockAnim)
			{
				auto skill = this->registry.create();

				auto weapon = this->registry.try_get<Weapon>(e);
				if (weapon)
				{
					this->registry.emplace<Skill>(skill, e, fixed16(0.5));
					this->registry.emplace<SkillAttacthBone>(skill, skill_state.name, "RightWeapon");
					this->registry.emplace<BoundingBox>(skill, weapon->bounding_box);

					auto transform = this->registry.try_get<Transform>(e);
					if (transform)
					{
						this->registry.emplace<Transform>(skill, *transform);
					}
				}
			}
		});
	}

	~SkillStateSystem()
	{

	}

	bool Initialize() override
	{
		return true;
	}

	void Update(fixed16 dt)
	{
		OnTransition(dt);
		OnExit();
		OnEnter();
		ProcessCurState();
	}

	template<class T>
	bool CheckCondition(T value, T target, SkillGraphInfo::Skill::Transition::Condition::CheckType check_type) const
	{
		switch (check_type)
		{
		case SkillGraphInfo::Skill::Transition::Condition::CheckType::kNone:
			assert(false);
			return false;

		case SkillGraphInfo::Skill::Transition::Condition::CheckType::kGreater:
			return value > target;

		case SkillGraphInfo::Skill::Transition::Condition::CheckType::kLess:
			return value < target;

		case SkillGraphInfo::Skill::Transition::Condition::CheckType::kEquals:
			return value == target;

		case SkillGraphInfo::Skill::Transition::Condition::CheckType::kNotEquals:
			return value != target;

		case SkillGraphInfo::Skill::Transition::Condition::CheckType::kGreaterAndEquals:
			return value >= target;

		case SkillGraphInfo::Skill::Transition::Condition::CheckType::kLessAndEquals:
			return value <= target;

		case SkillGraphInfo::Skill::Transition::Condition::CheckType::kTrue:
			return value == target;

		case SkillGraphInfo::Skill::Transition::Condition::CheckType::kFalse:
			return value == target;
		}

		return false;
	}

	

	void OnTransition(fixed16 dt)
	{
		auto view = registry.view<SkillState,SkillGraph,SkillParams, AnimationClip>(entt::exclude<ExitSkillState, EnterSkillState>);
		for (auto e : view)
		{
			auto& skill_state = view.get<SkillState>(e);
			const auto& skill_graph = view.get<SkillGraph>(e);
			const auto& params = view.get<SkillParams>(e);
			const auto& animation_clip = view.get<AnimationClip>(e);
			
			auto& skill = skill_graph.value->GetSkillByName(skill_state.name);
			for (auto& transition : skill.transitions())
			{
				bool can_trans = true;
				for (auto& condition : transition.conditions())
				{
					auto iter = params.value.find(condition.name());
					if (iter == params.value.end())
					{
						can_trans = false;
						break;
					}

					auto type = condition.condition_type();
					switch (type)
					{
					case SkillGraphInfo::Skill::Transition::Condition::ConditionType::kNone:
						assert(false);
						break;

					case SkillGraphInfo::Skill::Transition::Condition::ConditionType::kInt:
						can_trans = CheckCondition(iter->second.int_value, condition.value().int_value, condition.check_type());
						break;
					case SkillGraphInfo::Skill::Transition::Condition::ConditionType::kFloat:
						can_trans = CheckCondition(iter->second.float_value, condition.value().float_value, condition.check_type());
						break;
					case SkillGraphInfo::Skill::Transition::Condition::ConditionType::kBool:
						can_trans = CheckCondition(iter->second.bool_value, condition.value().bool_value, condition.check_type());
						break;
					case SkillGraphInfo::Skill::Transition::Condition::ConditionType::kAnim:
						if (!skill.anim_name().empty())
						{
							can_trans = animation_clip.is_done;
						}
						else
						{
							can_trans = false;
						}
						break;
					default:
						break;
					}
					
					if (!can_trans)
					{
						break;
					}
				}

				if (can_trans)
				{
					registry.emplace<ExitSkillState>(e, skill.name());
					registry.emplace<EnterSkillState>(e, transition.next_skill());
					break;
				}
				else
				{
					if (animation_clip.is_done)
					{
						registry.remove<SkillState>(e);
					}
				}
			}
		}
	}

	void OnExit()
	{
		auto view = registry.view<ExitSkillState>();
		for (auto e : view)
		{
			//什么也不做
		}
	}

	void OnEnter()
	{
		auto view = registry.view<EnterSkillState,SkillGraph>();
		for (auto e : view)
		{
			auto& enter_skill = view.get<EnterSkillState>(e);
			const auto& skill_graph = view.get<SkillGraph>(e);

			auto& skill = skill_graph.value->GetSkillByName(enter_skill.name);
			registry.emplace_or_replace<AnimationClip>(e,skill.anim_name());
			registry.emplace_or_replace<SkillState>(e, enter_skill.name);
		}
	}

	void ProcessCurState()
	{
		auto view = registry.view<SkillState, SkillGraph, SkillParams, AnimationClip>();
		for (auto e : view)
		{
			const auto& skill_graph = view.get<SkillGraph>(e);
			auto& skill_state = view.get<SkillState>(e);
			auto& skill_params = view.get<SkillParams>(e);
			const auto& animation_clip = view.get<AnimationClip>(e);

			auto skill = skill_graph.value->GetSkillByName(skill_state.name);
			for (auto& anim_event : skill.anim_events())
			{
				if (animation_clip.time >= anim_event.time() && skill_state.executed_events.find(anim_event.name()) == skill_state.executed_events.end())
				{
					auto iter = events.find(anim_event.name());
					if (iter != events.end())
					{
						iter->second(e, skill_graph, skill_state, skill_params);
						skill_state.executed_events.emplace(anim_event.name());
					}
				}
			}
		}
	}

	void LateUpdate(fixed16 dt) override
	{
		registry.clear<ExitSkillState>();
		registry.clear<EnterSkillState>();
	}

	void Finalize() override
	{

	}
};