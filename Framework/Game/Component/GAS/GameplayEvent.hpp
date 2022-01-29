#pragma once

#include <vector>
#include <unordered_map>
#include <variant>

#include "Framework/Game/GAS/GameplayTag.hpp"

#include "3rdparty/include/entt/entt.hpp"

#include "Kanex.hpp"

using GameplayEventData = std::variant<entt::entity, std::vector<entt::entity>>;

struct GameplayEvent
{
	GameplayTag tag;
	GameplayEventData data;

	GameplayEvent(const GameplayTag& _tag,const GameplayEventData& _data)
		: tag(_tag)
		, data(_data)
	{

	}
};

//得保证在当前在处理完
//不然需要在同步这个数据
//因为在回滚的时候这个需要被恢复
//如果需要回滚这个数据的话key应该使用Local
struct GameplayEventSendManager
{
	std::unordered_map<entt::entity, std::vector<GameplayEvent>> senders;
};

struct GameplayWaitEventSpec
{
	GameplayTag tag;
	std::string handler;

	GameplayWaitEventSpec() = default;

	GameplayWaitEventSpec(const GameplayTag& _tag, std::string_view _handler)
		: tag(_tag)
		, handler(_handler)
	{
		
	}
};

NON_MEMEBER_BAR(GameplayWaitEventSpec, obj.tag, obj.handler);

struct GameplayWaitEventTask
{
	std::unordered_map<GameplayTag, GameplayWaitEventSpec> value;
};

NON_MEMEBER_BAR(GameplayWaitEventTask, obj.value);