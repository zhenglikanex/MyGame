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

struct GameplayEventSendManager
{
	std::unordered_map<Local, std::vector<GameplayEvent>> senders;
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