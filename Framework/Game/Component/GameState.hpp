#pragma once

#include <cstdint>

#include "Kanex.hpp"

struct GameState
{
	GameState()
		: run_frame(0)
	{

	}

	uint32_t run_frame;		// 当前运行帧,包含预测帧
};

NON_MEMEBER_BAR(GameState,obj.run_frame);