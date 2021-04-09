#pragma once

#include "Actor.h"
#include "ActorMessage.h"
#include "NetworkComponent.hpp"

using namespace actor_net;

class KcpGateActor : public Actor
{
public:
	KcpGateActor(ActorId id);
	virtual ~KcpGateActor();

	bool Init(const std::shared_ptr<ActorNet>& actor_net) override;
	void Stop() override;

	void Receive(ActorMessage&& actor_msg) override;
	void NetworkReceive(ActorMessage&& actor_msg);
private:
	std::shared_ptr<NetworkComponent> network_component_;
};