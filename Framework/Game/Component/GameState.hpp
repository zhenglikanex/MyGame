#pragma once

#include <cstdint>

#include "Kanex.hpp"

struct GameState
{
	GameState()
		: run_frame(0)
	{

	}

	uint32_t run_frame;		// ��ǰ����֡,����Ԥ��֡
};

NON_MEMEBER_BAR(GameState,obj.run_frame);