#include "BattleActor.hpp"
#include <iostream>

BattleActor::BattleActor(ActorId id)
	: Actor(id)
{

}

BattleActor::~BattleActor()
{

}

bool BattleActor::Init(const std::shared_ptr<ActorNet>& actor_net)
{
	if (!Actor::Init(actor_net))
	{
		return false;
	}

	std::cout << "BattleActor start!!!" << std::endl;
	return true;
}

void BattleActor::Stop()
{

}

void BattleActor::Receive(ActorMessage&& actor_msg)
{
}

void BattleActor::Start(const std::any& data)
{
	players_ = std::any_cast<std::vector<ActorId>>(data);
	std::cout << "Battle Start Success!" << std::endl;
	//std::cout << "AgentStart" << gate << " " << conv << std::endl;
}

ACTOR_IMPLEMENT(BattleActor)