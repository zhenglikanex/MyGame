#pragma once
#include "IActor.h"
#include "ActorMessage.h"

using namespace actor_net;

class StartActor : public IActor
{
public:
	StartActor();
	virtual ~StartActor();

	bool Init(const std::shared_ptr<ActorNet>& actor_net_ptr) override;
	void Stop() override;

	void OnReceive(ActorMessage&& message) override;
private:

};
