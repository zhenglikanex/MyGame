#pragma once
#include "IActor.h"
#include "ActorMessage.h"

using namespace actor_net;

class MasterActor : public IActor
{
public:
	MasterActor();
	virtual ~MasterActor();

	bool Init(const std::shared_ptr<ActorNet>& actor_net) override;
	void Stop() override;

	void OnReceive(const ActorMessagePtr& actor_msg) override;
private:

};