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

	void Start(const std::any& data);
	void StartBattle();
	void Update(uint32_t dt);
	void InputPlayerCommand(ActorId id, const Proto::GameCommond& command);
	void PushPlayerCommand();
private:
	std::vector<ActorId> players_;
	std::vector<Proto::GameCommondGroup> command_groups_;
	bool start_;
	uint32_t last_time_;
	uint32_t time_;
	uint32_t frame_;
};