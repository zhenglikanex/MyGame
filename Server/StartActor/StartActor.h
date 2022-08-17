#pragma once
#include "Actor.h"
#include "ActorMessage.h"

using namespace actor_net;

class StartActor : public Actor
{
public:
	explicit StartActor(uint32_t id);

	bool Init(const std::shared_ptr<ActorNet>& actor_net_ptr) override;
	void Stop() override;

	void Receive(ActorMessage&& message) override;
private:

};
