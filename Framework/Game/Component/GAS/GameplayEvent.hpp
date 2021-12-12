#pragma once

#include <vector>
#include <unordered_map>

#include "Framework/Game/GAS/GameplayTag.hpp"

#include "3rdparty/include/entt/entt.hpp"

struct GameplayEventSender
{
	GameplayTag tag;
	void* data;

	GameplayEventSender(const GameplayTag& _tag, void* _data)
		: tag(_tag)
		, data(_data)
	{

	}
};

struct GameplayEventReceiver
{
	std::function<void(void*)> callback;

	GameplayEventReceiver(const std::function<void(void*)>& _callback)
		: callback(_callback)
	{

	}
};

using GameplayEventReceiverGroup = std::unordered_map<GameplayTag, std::vector<GameplayEventReceiver>>;

struct GameplayEvent
{
	std::vector<GameplayEventSender> senders;
	std::unordered_map<entt::entity, GameplayEventReceiverGroup> receivers;
};