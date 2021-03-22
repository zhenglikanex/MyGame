#pragma once

#include "Kanex.hpp"

enum class ActorStateType : uint8_t
{
	kIdle,
	kMovement,
	kJump,
	kAttack,
	kHurt,
	kDeath,

	kMax
};

struct ActorState
{
	ActorStateType cur_state;
	fixed16 time;

	ActorState() : cur_state(ActorStateType::kIdle),time(fixed16(0))
	{
		
	}

	ActorState(ActorStateType state) : cur_state(state),time(fixed16(0))
	{

	}
};

struct EnterActorState
{
	ActorStateType value;

	EnterActorState()
		: value(ActorStateType::kMax)
	{

	}

	EnterActorState(ActorStateType _value)
		:value(_value)
	{

	}
};

struct ExitActorState
{
	ActorStateType value;

	ExitActorState()
		: value(ActorStateType::kMax)
	{

	}

	ExitActorState(ActorStateType _value)
		:value(_value)
	{

	}
};

NON_MEMEBER_BAR(ActorState, obj.cur_state, obj.time);
NON_MEMEBER_BAR(ExitActorState,obj.value);
NON_MEMEBER_BAR(EnterActorState, obj.value);