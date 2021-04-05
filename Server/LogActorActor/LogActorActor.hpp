#pragma once
#include "IActor.h"
#include "ActorMessage.h"

using namespace actor_net;

class LogActorActor : public IActor
{
public:
	LogActorActor();
	virtual ~LogActorActor();

	bool Init(const std::shared_ptr<ActorNet>& actor_net) override;
	void Stop() override;

	void OnReceive(ActorMessage&& actor_msg) override;
private:

};