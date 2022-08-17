#pragma once


#include "Framework/Game/Component/GAS/GameplayEvent.hpp"

namespace gameplay_event_utility
{
	static void SendGameplayEvent(Registry& registry,entt::entity e,const GameplayTag& tag, const GameplayEventData& data)
	{
		auto& manager = registry.ctx_or_set<GameplayEventSendManager>();
		auto& sender = manager.senders[e];
		sender.emplace_back(tag, data);
	}
};