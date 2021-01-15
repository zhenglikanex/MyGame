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
};


struct ActionState
{
	ActionStateType last_state;
	ActionStateType cur_state;
	float
};
