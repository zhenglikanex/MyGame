#pragma once
#include "Actor.h"
#include "ActorMessage.h"

using namespace actor_net;

class MatchActor : public Actor
{
public:
	MatchActor(ActorId);
	virtual ~MatchActor();

	bool Init(const std::shared_ptr<ActorNet>& actor_net) override;
	void Stop() override;

	void Receive(ActorMessage&& actor_msg) override;

	void Match();

	void JoinMatch(const std::any& data);
	void LeaveMatch(const std::any& data);
private:
	std::unordered_map<ActorId,ActorId> matching_agent_;
};