#pragma once

#include <vector>
#include <unordered_map>

#include "Framework/Game/GAS/GameplayTag.hpp"

#include "3rdparty/include/entt/entt.hpp"

struct GameplayEventReceiver
{
	std::function<void(void*)> callback;

	GameplayEventReceiver(const std::function<void(void*)>& _callback)
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
	void* data;

	GameplayEvent(const GameplayTag& _tag, void* _data)
		: tag(_tag)
		, data(_data)
	{

	}
};

struct GameplayEventSender
{
	std::vector<GameplayEvent> events;
};
