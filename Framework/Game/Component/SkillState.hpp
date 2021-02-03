#pragma once

#include "Framework/Game/FixedPoint.hpp"

#include "Framework/serialize/binary_archive.hpp"

struct SkillState
{
	uint32_t cur_state;
	fixed16 time;

	SkillState(uint32_t state) : cur_state(state), time(fixed16(0))
	{

	}

	BAR(cur_state, time);
};

struct EnterSkillState
{
	uint32_t value;
};

struct ExitSkillState
{
	uint32_t value;
};

struct SkillCommand
{
	uint32_t skill;
};

struct SkillStateMachine
{
	
};