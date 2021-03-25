#pragma once

#include <cstdint>

#include "Kanex.hpp"

struct GameState
{
	GameState()
		: real_frame(0), run_frame(0),run_time(0)
	{

	}

	uint32_t real_frame;	// 正确的运行帧，不包含预测，跟服务器统一
	uint32_t run_frame;		// 当前运行帧,包含预测帧
	uint32_t run_time;
};

NON_MEMEBER_BAR(GameState, obj.real_frame, obj.run_frame, obj.run_time);