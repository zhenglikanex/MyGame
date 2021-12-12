#pragma once

#include "Framework/Game/Component/GAS/GameplayEvent.hpp"

class GameplayEventUtility
{
public:
	static void ReceiveGameplayEvent(entt::registry& registry, entt::entity entity, const GameplayTag& tag, const std::function<void(void*)>& callback)
	{
		auto& gameplay_event = registry.ctx_or_set<GameplayEvent>();
		gameplay_event.receivers[entity][tag].emplace_back(callback);
	}

	static void SendGameplayEvent(entt::registry& registry,const GameplayTag& tag,void* data)
	{
		auto& gameplay_event = registry.ctx_or_set<GameplayEvent>();
		gameplay_event.senders.emplace()
	}
};