#pragma once

#include <vector>
#include <unordered_map>
#include <variant>

#include "Framework/Game/GAS/GameplayTag.hpp"

#include "3rdparty/include/entt/entt.hpp"

using GameplayEventData = std::variant<entt::entity, std::vector<entt::entity>>;

struct GameplayEventReceiver
{
	std::function<void(const GameplayEventData&)> callback;

	GameplayEventReceiver(const std::function<void(const GameplayEventData&)>& _callback)
		: callback(_callback)
	{

	}
};

struct GameplayEventReceiverMap
{
	std::unordered_map<GameplayTag,GameplayEventReceiver> value;
};

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

struct GameplayEventSender
{
	std::vector<GameplayEvent> events;
};
