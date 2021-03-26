#pragma once

#include <unordered_map>
#include <memory>

#include "Framework/Game/FixedPoint.hpp"

struct Command
{
	fixed16 x_axis;		// ҡ������
	fixed16 y_axis;		// ҡ������

	// ����������һЩ����,����Ծ,����ɶ��һЩ��λ�Ƿ���
	bool jump;
	uint32_t skill;

	Command()
		: x_axis(0)
		, y_axis(0)
		, jump(false)
		, skill(0)
	{
		
	}
};

NON_MEMEBER_BAR(Command, obj.x_axis, obj.y_axis, obj.jump, obj.skill);

