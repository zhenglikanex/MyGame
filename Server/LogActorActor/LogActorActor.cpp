#include "LogActorActor.hpp"

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

	return true;
}

void LogActorActor::Stop()
{

}

void LogActorActor::OnReceive(ActorMessage&& actor_msg)
{
	
}

ACTOR_IMPLEMENT(LogActorActor)