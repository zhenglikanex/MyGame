#pragma once

#include <cstdint>

#include "Kanex.hpp"

struct GameState
{
	uint32_t run_frame;		// ��ǰ����֡,����Ԥ��֡

	GameState()
		: run_frame(0)
	{

	}
};

NON_MEMEBER_BAR(GameState,obj.run_frame);