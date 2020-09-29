#pragma once

#include <unordered_map>
#include <memory>

#include "Framework/Game/FixedPoint.hpp"
#include "Framework/serialize/binary_archive.hpp"

struct Commond
{
	fixed16 x_axis;		// ҡ������
	fixed16 y_axis;		// ҡ������

	// ����������һЩ����,����Ծ,����ɶ��һЩ��λ�Ƿ���
	bool jump;
	bool attack;
	bool skill1;
	bool skill2;
	bool skill3;

	Commond()
	{
		Reset();
	}

	void Reset()
	{
		x_axis = fixed16(0);
		y_axis = fixed16(0);
		jump = false;
		attack = false;
		skill1 = false;
		skill2 = false;
		skill3 = false;
	}

	BAR(x_axis, y_axis, jump, attack, skill1, skill2, skill3)
};

struct MovementCommond
{
	fixed16 x_axis;
	fixed16 y_axis;

	BAR(x_axis, y_axis);
};

struct JumpCommond
{
	bool jump;
	BAR(jump);
};

struct AttackCommond
{
};

struct CommondGroup
{
	std::unordered_map<uint32_t, Commond> value;

	bool operator==(const CommondGroup& commonds)
	{
		// todo:
		return true;
	}
};