#pragma once
#include "IActor.h"
#include "ActorMessage.h"
#include "Network.h"

using namespace actor_net;

class HarborActor : public IActor
{
public:
	HarborActor();
	virtual ~HarborActor();

	bool Init(const std::shared_ptr<ActorNet>& actor_net) override;
	void Stop() override;

	void OnReceive(const ActorMessagePtr& actor_msg) override;
private:

};