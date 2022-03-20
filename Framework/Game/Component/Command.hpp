#pragma once

#include <string>
#include <unordered_map>
#include <memory>

#include "Framework/Game/GameConfig.hpp"
#include "Framework/Game/Component/MoveCommand.hpp"
#include "Framework/Game/Component/GAS/GameplayAbilityCommand.hpp"

//struct Command
//{
//	uint32_t frame;
//
//	//这个操作命令时的已经确认的帧,
//	//用于在服务端进行命中等一些判定时回滚到客户端所看到的状态
//	//进行延迟补偿
//	uint32_t verified_frame;
//
//	float x_axis;		// 摇杆输入
//	float y_axis;		// 摇杆输入
//
//	std::unordered_map<std::string, AbilityStatus> abilities;
//
//	Command()
//		: frame(0)
//		, verified_frame(0)
//		, x_axis(0)
//		, y_axis(0)
//	{
//		
//	}
//};

struct Command
{
	uint32_t frame;
	MoveCommand move_command;
	GameplayAbilityCommand gameplay_ability_command;
};

NON_MEMEBER_BAR(Command,obj.move_command,obj.gameplay_ability_command);

struct CommandState
{
	std::array<Command, GameConfig::kMaxPredictFrame> value;
};

