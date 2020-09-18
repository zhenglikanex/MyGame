#include "StartActor.h"
#include <iostream>

StartActor::StartActor()
{

}

StartActor::~StartActor()
{

}

bool StartActor::Init(const std::shared_ptr<ActorNet>& actor_net_ptr)
{
	if (!IActor::Init(actor_net_ptr))
	{
		return false;
	}

	std::cout << "StartActor start!!!" << std::endl;
	return true;
}

void StartActor::Stop()
{

}

void StartActor::OnReceive(const ActorMessagePtr& actor_msg)
{
	char* p = new char[actor_msg->size() + 1];
	std::memcpy(p, actor_msg->data().data(), actor_msg->size());
	p[actor_msg->size()] = '\0';

	std::cout << p << std::endl;
}

ACTOR_IMPLEMENT(StartActor)