#pragma once

#include <memory>

#include "Actor.h"

#include "Framework/Game/Game.hpp"

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
	void PushCommandGroup();
private:
	void StartBattle(const std::any& data);
	void InputCommand(const std::any& data);
private:
	void GameInput() const;
private:
	std::vector<ActorId> players_;
	std::unordered_map<ActorId, uint32_t> ids_;
	std::unordered_map<uint32_t, std::vector<Proto::GameCommand>> player_commands_;

	bool start_;
	uint32_t start_time_;
	std::unique_ptr<Game> game_;
};