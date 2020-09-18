#include "LogActorActor.h"

LogActorActor::LogActorActor()
{

}

LogActorActor::~LogActorActor()
{

}

bool LogActorActor::Init(const std::shared_ptr<ActorNet>& actor_net)
{
	if (!IActor::Init(actor_net))
	{
		return false;
	}

	std::cout << "LogActorActor start!!!" << std::endl;
	return true;
}

void LogActorActor::Stop()
{

}

void LogActorActor::OnReceive(const ActorMessagePtr& actor_msg)
{
	char* p = new char[actor_msg->size() + 1];
	std::memcpy(p, actor_msg->data().data(), actor_msg->size());
	p[actor_msg->size()] = 0;

	std::cout << p << std::endl;
}

ACTOR_IMPLEMENT(LogActorActor)