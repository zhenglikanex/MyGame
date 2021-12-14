#pragma once

#include "Framework/Game/Component/GAS/GameplayEvent.hpp"

class GameplayEventUtility
{
public:
	static void ReceiveGameplayEvent(entt::registry& registry, entt::entity entity, const GameplayTag& tag, const std::function<void(const GameplayEventData&)>& callback)
	{
		auto& receiver_map = registry.get_or_emplace<GameplayEventReceiverMap>(entity);
		receiver_map.value.emplace(tag, GameplayEventReceiver(callback));
	}

	static void SendGameplayEvent(entt::registry& registry,const GameplayTag& tag, const GameplayEventData& data)
	{
		auto& gameplay_event = registry.ctx_or_set<GameplayEventSender>();
		gameplay_event.events.emplace_back(tag, data);
	}

	static void RemoveReceiveGameplayEvent(entt::registry& registry,entt::entity entity,const GameplayTag& tag)
	{
		if (!registry.has<GameplayEventReceiverMap>(entity))
		{
			return;
		}

		auto& map = registry.get<GameplayEventReceiverMap>(entity);
		map.value.erase(tag);
	}
};