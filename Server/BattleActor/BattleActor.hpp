#pragma once
#include "Actor.h"
#include "ActorMessage.h"
#include "Framework/Proto/Battle.pb.h"

using namespace actor_net;

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
	const float kFrameTime = 0.03333334f;

	std::vector<ActorId> players_;
	std::unordered_map<ActorId, uint32_t> ids_;
	std::unordered_map<uint32_t, std::vector<Proto::GameCommand>> player_commands_;
	bool start_;
	uint32_t start_time_;
	float run_time_;
	uint32_t run_frame_;
};