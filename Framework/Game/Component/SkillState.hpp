#pragma once

#include <unordered_set>

#include "Framework/Game/FixedPoint.hpp"

#include "Framework/Game/Data/SkillGraph.hpp"

#include "Framework/serialize/binary_archive.hpp"

struct SkillState
{
	std::string name;
	fixed16 time;
	std::unordered_set<std::string> executed_events;

	SkillState(std::string_view _name) : name(_name), time(fixed16(0))
	{
	}

	//BAR(_name, time,executed_events);
};

struct EnterSkillState
{
	std::string name;

	EnterSkillState(std::string_view _name)
		:name(_name)
	{

	}
};

struct ExitSkillState
{
	std::string name;

	ExitSkillState(std::string_view _name)
		:name(_name)
	{

	}
};

struct SkillParams
{
	std::unordered_map<std::string, SkillGraphInfo::Param> value;

	SkillParams()
	{
	}
};

struct SkillGraph
{
	SkillGraphInfo* value;

	SkillGraph(SkillGraphInfo* _value)
		:value(_value)
	{

	}
};