#pragma once
#include "Actor.h"
#include "ActorMessage.h"

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
private:
	std::vector<ActorId> players_;
};