#pragma once

#include <cstdint>

#include "Kanex.hpp"

struct GameState
{
	uint32_t real_frame;		// 游戏从开始到现在过去的帧
	uint32_t run_frame;			// 当前运行到的帧
	uint32_t verified_frame;	// 确认的帧

	GameState()
		: real_frame(0)
		, run_frame(0)
		, verified_frame(0)
	{

	}
};

NON_MEMEBER_BAR(GameState,obj.run_frame,obj.verified_frame);