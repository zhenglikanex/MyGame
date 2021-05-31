#pragma once

#include <unordered_map>
#include <memory>

#include "Framework/Game/FixedPoint.hpp"

struct Command
{
	fixed16 x_axis;		// 摇杆输入
	fixed16 y_axis;		// 摇杆输入

	// 其他按键的一些输入,如跳跃,攻击啥的一些键位是否按下
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

struct CommandGroup
{
	uint32_t frame;
	std::unordered_map<uint32_t, Command> commands;

	CommandGroup()
		: frame(std::numeric_limits<uint32_t>::max())
	{

	}
};

NON_MEMEBER_BAR(Command, obj.x_axis, obj.y_axis, obj.jump, obj.skill);
NON_MEMEBER_BAR(CommandGroup, obj.frame, obj.commands);

