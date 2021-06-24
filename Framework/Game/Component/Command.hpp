#pragma once

#include <unordered_map>
#include <memory>

#include "Framework/Game/FixedPoint.hpp"

struct Command
{
	uint32_t frame;

	fixed16 x_axis;		// 摇杆输入
	fixed16 y_axis;		// 摇杆输入

	// 其他按键的一些输入,如跳跃,攻击啥的一些键位是否按下
	bool jump;
	uint32_t skill;

	Command()
		: frame(0)
		, x_axis(0)
		, y_axis(0)
		, jump(false)
		, skill(0)
	{
		
	}

	bool operator==(const Command& rhs) const
	{
		return x_axis == rhs.x_axis && y_axis == rhs.y_axis && jump == rhs.jump && skill == rhs.skill;
	}

	bool operator!=(const Command& rhs) const
	{
		return !(*this == rhs);
	}
};

struct CommandGroup
{
	uint32_t frame;
	std::unordered_map<uint32_t, Command> commands;

	CommandGroup()
		: frame(0)
	{

	}
};

NON_MEMEBER_BAR(Command,obj.frame, obj.x_axis, obj.y_axis, obj.jump, obj.skill);
NON_MEMEBER_BAR(CommandGroup, obj.frame, obj.commands);

