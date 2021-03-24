#pragma once

#include <unordered_set>

#include "Framework/Game/FixedPoint.hpp"

#include "Framework/Game/Data/SkillGraph.hpp"

#include "Kanex.hpp"

struct SkillState
{
	std::string name;
	fixed16 time;
	std::unordered_set<std::string> executed_events;

	SkillState() = default;

	SkillState(std::string_view _name) 
		: name(_name)
		, time(fixed16(0))
	{

	}
};

struct EnterSkillState
{
	std::string name;

	EnterSkillState() = default;

	EnterSkillState(std::string_view _name)
		:name(_name)
	{

	}
};

struct ExitSkillState
{
	std::string name;

	ExitSkillState() = default;

	ExitSkillState(std::string_view _name)
		: name(_name)
	{

	}
};

struct SkillParams
{
	std::unordered_map<std::string, SkillGraphInfo::Param> value;

	SkillParams() = default;
};

struct SkillGraph
{
	SkillGraphInfo* value;

	SkillGraph(SkillGraphInfo* _value)
		:value(_value)
	{

	}
};

NON_MEMEBER_BAR(SkillState, obj.name, obj.time, obj.executed_events);
NON_MEMEBER_BAR(EnterSkillState, obj.name);
NON_MEMEBER_BAR(ExitSkillState, obj.name);
NON_MEMEBER_BAR(SkillParams, obj.value);