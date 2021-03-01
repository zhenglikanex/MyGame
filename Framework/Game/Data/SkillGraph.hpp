#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "Framework/Game/Math.hpp"

class SkillGraphInfo
{
public:
	friend void from_json(const json& j, SkillGraphInfo& skill_graph);

	struct Param
	{
		Param() 
			:int_value(0)
		{
		}

		union
		{
			int int_value;
			fixed16 float_value;
			bool bool_value;
		};
	};

	class Skill
	{
	public:
		friend void from_json(const json& j, Skill& skill);

		class Transition
		{
		public:
			friend void from_json(const json& j, Transition& transition);

			class Condition
			{
			public:
				friend void from_json(const json& j, Condition& condition);

				enum class ConditionType
				{
					kNone,
					kInt,
					kFloat,
					kBool,
					kAnim
				};

				enum class CheckType
				{
					kNone,
					kGreater, 
					kLess, 
					kEquals,
					kNotEquals, 
					kGreaterAndEquals, 
					kLessAndEquals, 
					kTrue, 
					kFalse
				};

				Condition()
					: condition_type_(ConditionType::kNone)
					, check_type_(CheckType::kNone)
				{

				}

				const std::string& name() const { return name_; }
				ConditionType condition_type() const { return condition_type_; }
				CheckType check_type() const { return check_type_; }
				Param value() const { return value_; }
			private:
				std::string name_;
				ConditionType condition_type_;
				CheckType check_type_;
				Param value_;
			};

			const std::string& next_skill() const { return next_skill_; }
			const std::vector<Condition>& conditions() const { return conditions_; }
		private:
			std::string next_skill_;
			std::vector<Condition> conditions_;
		};

		class AnimEvent
		{
		public:
			friend void from_json(const json& j, AnimEvent& anim_event);

			AnimEvent()
				:time_(0)
			{}

			const std::string& name() const { return name_; }
			fixed16 time() const { return time_; }
		private:
			std::string name_;
			fixed16 time_;
		};

		const std::string& name() const { return name_; }
		const std::string& anim_name() const { return anim_name_; }
		const std::vector<AnimEvent>& const anim_events() const { return anim_events_; }
		const std::vector<Transition>& transitions() const { return transitions_; }
	private:
		std::string name_;
		std::string anim_name_;
		std::vector<AnimEvent> anim_events_;
		std::vector<Transition> transitions_;
	};

	bool HasSkillByName(const std::string& name) const;
	const Skill& GetSkillByName(const std::string& name) const;

	const std::string& entry() const { return entry_; }
	const std::unordered_map<std::string, Skill>& skills() const { return skills_; }
private:
	std::string entry_;	// 默认进入的状态
	std::unordered_map<std::string, Skill> skills_;
};

inline void from_json(const json& j, SkillGraphInfo::Skill::Transition::Condition& condition)
{
	j.at("name").get_to(condition.name_);

	std::string str;
	// condition_type
	j.at("type").get_to(str);
	if (str == "int")
	{
		condition.condition_type_ = SkillGraphInfo::Skill::Transition::Condition::ConditionType::kInt;
	}
	else if (str == "float")
	{
		condition.condition_type_ = SkillGraphInfo::Skill::Transition::Condition::ConditionType::kFloat;
	}
	else if (str == "bool")
	{
		condition.condition_type_ = SkillGraphInfo::Skill::Transition::Condition::ConditionType::kBool;
	}
	else if (str == "anim")
	{
		condition.condition_type_ = SkillGraphInfo::Skill::Transition::Condition::ConditionType::kAnim;
		return;
	}

	// check_type
	j.at("check_type").get_to(str);
	if (str == "greater")
	{
		condition.check_type_ = SkillGraphInfo::Skill::Transition::Condition::CheckType::kGreater;
	}
	else if (str == "less")
	{
		condition.check_type_ = SkillGraphInfo::Skill::Transition::Condition::CheckType::kLess;
	}
	else if (str == "equals")
	{
		condition.check_type_ = SkillGraphInfo::Skill::Transition::Condition::CheckType::kEquals;
	}
	else if (str == "not_equals")
	{
		condition.check_type_ = SkillGraphInfo::Skill::Transition::Condition::CheckType::kNotEquals;
	}
	else if (str == "greater_and_equals")
	{
		condition.check_type_ = SkillGraphInfo::Skill::Transition::Condition::CheckType::kGreaterAndEquals;
	}
	else if (str == "less_and_equals")
	{
		condition.check_type_ = SkillGraphInfo::Skill::Transition::Condition::CheckType::kLessAndEquals;
	}
	else if (str == "true")
	{
		condition.check_type_ = SkillGraphInfo::Skill::Transition::Condition::CheckType::kTrue;
	}
	else if (str == "false")
	{
		condition.check_type_ = SkillGraphInfo::Skill::Transition::Condition::CheckType::kFalse;
	}

	// value
	j.at("value").get_to(str);
	switch (condition.condition_type_)
	{
	case SkillGraphInfo::Skill::Transition::Condition::ConditionType::kInt:
		condition.value_.int_value = std::stoi(str);
		break;
	case SkillGraphInfo::Skill::Transition::Condition::ConditionType::kFloat:
		condition.value_.float_value = fixed16(str);
		break;
	case SkillGraphInfo::Skill::Transition::Condition::ConditionType::kBool:
		condition.value_.bool_value = str == "true" ? true : false;
		break;
	case SkillGraphInfo::Skill::Transition::Condition::ConditionType::kAnim:
		break;
	default:
		assert(false && "not condition type!");
		break;
	}
}

inline void from_json(const json& j, SkillGraphInfo::Skill::Transition& transition)
{
	j.at("name").get_to(transition.next_skill_);
	j.at("conditions").get_to(transition.conditions_);
}

inline void from_json(const json& j, SkillGraphInfo::Skill::AnimEvent& anim_event)
{
	j.at("name").get_to(anim_event.name_);
	j.at("time").get_to(anim_event.time_);
}

inline void from_json(const json& j, SkillGraphInfo::Skill& skill)
{
	if (j.at("anim_name").is_string())
	{
		j.at("anim_name").get_to(skill.anim_name_);
	}
	j.at("anim_events").get_to(skill.anim_events_);
	j.at("transitions").get_to(skill.transitions_);
}

inline void from_json(const json& j, SkillGraphInfo& skill_graph)
{
	j.at("entry").get_to(skill_graph.entry_);
	j.at("states").get_to(skill_graph.skills_);
}
