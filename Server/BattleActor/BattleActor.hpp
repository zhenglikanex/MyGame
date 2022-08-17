#pragma once

#include <memory>
#include <queue>
#include <optional>

#include "Actor.h"

#include "Framework/Game/Game.hpp"
#include "Framework/Game/Component/Command.hpp"

#include "Server/BattleActor/ServerDebugService.hpp"

using namespace actor_net;

inline std::unique_ptr<DebugService> g_debug_service = std::make_unique<ServerDebugService>();

class BattleActor : public Actor
{
public:
	explicit BattleActor(ActorId id);

	bool Init(const std::shared_ptr<ActorNet>& actor_net) override;
	void Stop() override;

	void Receive(ActorMessage&& actor_msg) override;

	void Update();
private:
	void StartBattle(const std::any& data);
	void PushGameMessage(const std::any& data);
private:
	std::optional<std::vector<uint8_t>> RecvGameMessage(const std::string& name);
	void SendGameMessage(uint32_t player, std::string_view name, std::vector<uint8_t>&& data);
private:
	std::vector<ActorId> players_;
	std::unordered_map<ActorId, uint32_t> ids_;

	bool start_;
	uint64_t last_time_;
	std::unique_ptr<Game> game_;
	std::unordered_map<std::string, std::queue<std::vector<uint8_t>>> game_messages_;
};