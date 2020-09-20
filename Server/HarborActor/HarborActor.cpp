#include "HarborActor.hpp"

HarborActor::HarborActor()
{

}

HarborActor::~HarborActor()
{

}

bool HarborActor::Init(const std::shared_ptr<ActorNet>& actor_net)
{
	if (!IActor::Init(actor_net))
	{
		return false;
	}

	std::cout << "HarborActor start!!!" << std::endl;
	return true;
}

void HarborActor::Stop()
{

}

void HarborActor::OnReceive(const ActorMessagePtr& actor_msg)
{
	char* p = new char[actor_msg->size() + 1];
	std::memcpy(p, actor_msg->data().data(), actor_msg->size());
	p[actor_msg->size()] = 0;

	std::cout << p << std::endl;
}

ACTOR_IMPLEMENT(HarborActor)