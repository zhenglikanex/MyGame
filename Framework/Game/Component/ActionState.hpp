#pragma once

#include "Framework/serialize/binary_archive.hpp"

enum class ActionStateType : uint8_t
{
	kIdle,
	kMovement,
	kJump,
	kAttack,
	kHurt,
	kDeath,

	kMax
};

struct ActionState
{
	ActionStateType cur_state;
	fixed16 time;

	ActionState() : cur_state(ActionStateType::kIdle),time(fixed16(0))
	{
		
	}

	ActionState(ActionStateType state) : cur_state(state),time(fixed16(0))
	{

	}

	BAR(cur_state, time);
};

struct EnterActionState
{
	ActionStateType value;
};

struct ExitActionState
{
	ActionStateType value;
};