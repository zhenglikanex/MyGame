#pragma once

#include <string>
#include <unordered_map>
#include <memory>

enum class AbilityStatus : uint8_t
{
	kDown,
	kUp,
};

struct Command
{
	uint32_t frame;

	//这个操作命令时的已经确认的帧,
	//用于在服务端进行命中等一些判定时回滚到客户端所看到的状态
	//进行延迟补偿
	uint32_t verified_frame;

	float x_axis;		// 摇杆输入
	float y_axis;		// 摇杆输入

	std::unordered_map<std::string, AbilityStatus> abilities;

	Command()
		: frame(0)
		, verified_frame(0)
		, x_axis(0)
		, y_axis(0)
	{
		
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

NON_MEMEBER_BAR(Command, obj.frame, obj.verified_frame, obj.x_axis, obj.y_axis,obj.abilities);
NON_MEMEBER_BAR(CommandGroup, obj.frame, obj.commands);

