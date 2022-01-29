#pragma once


#include "Framework/Game/Component/GAS/GameplayEvent.hpp"

class GameplayEventUtility
{
public:
	static void SendGameplayEvent(Registry& registry,const Local& local,const GameplayTag& tag, const GameplayEventData& data)
	{
		auto& manager = registry.ctx_or_set<GameplayEventSendManager>();
		auto& sender = manager.senders[local];
		sender.emplace_back(tag, data);
	}
};