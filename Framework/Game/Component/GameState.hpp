#pragma once

#include <cstdint>

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