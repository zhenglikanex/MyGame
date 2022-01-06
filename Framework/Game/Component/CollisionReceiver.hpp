#pragma once

#include "Framework/Game/Component/Local.hpp"

#include "Kanex.hpp"

struct CollisionReceiver
{
	Local value;

	CollisionReceiver() = default;
	CollisionReceiver(const Local& _value)
		: value(_value)
	{

	}
};

NON_MEMEBER_BAR(CollisionReceiver, obj.value);
