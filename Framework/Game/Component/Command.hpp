#pragma once

#include <unordered_map>
#include <memory>

#include "Framework/Game/FixedPoint.hpp"
#include "Framework/serialize/binary_archive.hpp"

struct Command
{
	fixed16 x_axis;		// ҡ������
	fixed16 y_axis;		// ҡ������

	// ����������һЩ����,����Ծ,����ɶ��һЩ��λ�Ƿ���
	bool jump;
	uint32_t skill;

	Command()
	{
		Reset();
	}

	void Reset()
	{
		x_axis = fixed16(0);
		y_axis = fixed16(0);
		jump = false;
		skill = 0;
	}

	BAR(x_axis, y_axis, jump,skill)
};

struct Movementcommand
{
	fixed16 x_axis;
	fixed16 y_axis;

	BAR(x_axis, y_axis);
};

struct Jumpcommand
{
	bool jump;
	BAR(jump);
};

struct Attackcommand
{
};
