#include "EmptyActor.h"

EmptyActor::EmptyActor()
{

}

EmptyActor::~EmptyActor()
{

}

bool EmptyActor::Init(const std::shared_ptr<ActorNet>& actor_net)
{
	if (!IActor::Init(actor_net))
	{
		return false;
	}

	std::cout << "EmptyActor start!!!" << std::endl;
	return true;
}

void EmptyActor::Stop()
{

}

void EmptyActor::OnReceive(const ActorMessagePtr& actor_msg)
{
	char* p = new char[actor_msg->size() + 1];
	std::memcpy(p, actor_msg->data().data(), actor_msg->size());
	p[actor_msg->size()] = 0;

	std::cout << p << std::endl;
}

ACTOR_IMPLEMENT(EmptyActor)