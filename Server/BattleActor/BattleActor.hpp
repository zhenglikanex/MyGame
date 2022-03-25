#pragma once

#include <memory>
#include <queue>

#include "Actor.h"

#include "Framework/Game/System.hpp"
#include "Framework/Game/Game.hpp"
#include "Framework/Game/Component/Command.hpp"

#include "Framework/Proto/Battle.pb.h"

#include "Server/BattleActor/ServerDebugService.hpp"

using namespace actor_net;

std::unique_ptr<DebugService> g_debug_service = std::make_unique<ServerDebugService>();

class BattleActor : public Actor
{
public:
	BattleActor(ActorId id);
	virtual ~BattleActor();

	bool Init(const std::shared_ptr<ActorNet>& actor_net) override;
	void Stop() override;

	void Receive(ActorMessage&& actor_msg) override;

	void Update();
private:
	void StartBattle(const std::any& data);
	void PushGameMessage(const std::any& data);
private:
	std::unique_ptr<std::vector<uint8_t>> RecvGameMessage(const std::string& name);
	void SendGameMessage(uint32_t player, std::string_view name, std::vector<uint8_t>&& data);
private:
	std::vector<ActorId> players_;
	std::unordered_map<ActorId, uint32_t> ids_;

	bool start_;
	uint64_t last_time_;
	std::unique_ptr<Game> game_;
	std::unordered_map<std::string, std::queue<std::unique_ptr<std::vector<uint8_t>>>> game_messages_;
};