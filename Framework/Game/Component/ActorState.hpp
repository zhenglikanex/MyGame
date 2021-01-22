#pragma once

#include "Framework/serialize/binary_archive.hpp"

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

	BAR(cur_state, time);
};

struct EnterActorState
{
	ActorStateType value;
};

struct ExitActorState
{
	ActorStateType value;
};