#pragma once

#include <unordered_map>
#include <memory>

#include "Framework/Game/FixedPoint.hpp"
#include "Framework/serialize/binary_archive.hpp"

struct Command
{
	fixed16 x_axis;		// 摇杆输入
	fixed16 y_axis;		// 摇杆输入

	// 其他按键的一些输入,如跳跃,攻击啥的一些键位是否按下
	bool jump;
	bool attack;
	bool skill1;
	bool skill2;
	bool skill3;

	Command()
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

struct commandGroup
{
	std::unordered_map<uint32_t, Command> value;

	bool operator==(const commandGroup& commands)
	{
		// todo:
		return true;
	}
};