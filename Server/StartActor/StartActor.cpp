#include "StartActor.h"
#include "ActorNet.h"
#include <iostream>

StartActor::StartActor(uint32_t id)
	:Actor(id)
{

}

StartActor::~StartActor()
{

}

bool StartActor::Init(const std::shared_ptr<ActorNet>& actor_net_ptr)
{
	if (!Actor::Init(actor_net_ptr))
	{
		return false;
	}

	std::cout << "StartActor start!!!" << std::endl;
	return true;
}

void StartActor::Stop()
{

}

void StartActor::Receive(ActorMessage&& actor_msg)
{
	
}

ACTOR_IMPLEMENT(StartActor)