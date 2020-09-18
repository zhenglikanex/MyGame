#pragma once
#include "IActor.h"
#include "ActorMessage.h"

using namespace actor_net;

class EmptyActor : public IActor
{
public:
	EmptyActor();
	virtual ~EmptyActor();

	bool Init(const std::shared_ptr<ActorNet>& actor_net) override;
	void Stop() override;

	void OnReceive(const ActorMessagePtr& actor_msg) override;
private:

};