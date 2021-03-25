#pragma once

#include <cstdint>

#include "Kanex.hpp"

struct GameState
{
	GameState()
		: real_frame(0), run_frame(0),run_time(0)
	{

	}

	uint32_t real_frame;	// ��ȷ������֡��������Ԥ�⣬��������ͳһ
	uint32_t run_frame;		// ��ǰ����֡,����Ԥ��֡
	uint32_t run_time;
};

NON_MEMEBER_BAR(GameState, obj.real_frame, obj.run_frame, obj.run_time);