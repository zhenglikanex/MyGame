#include "MatchActor.hpp"

MatchActor::MatchActor(ActorId id)
	: Actor(id)
{

}

MatchActor::~MatchActor()
{

}

bool MatchActor::Init(const std::shared_ptr<ActorNet>& actor_net)
{
	if (!Actor::Init(actor_net))
	{
		return false;
	}

	AddTimer(100, -1, [this]()
	{
		Match();
	});

	ActorConnect("join_match", &MatchActor::JoinMatch, this);
	ActorConnect("leave_match", &MatchActor::LeaveMatch, this);

	return true;
}

void MatchActor::Stop()
{

}

void MatchActor::Receive(ActorMessage&& actor_msg)
{

}

void MatchActor::Match()
{
	if (matching_agent_.size() >= 2)
	{
		// todo 创建战斗服务
		auto id = StartActor("BattleActor.dll");
		if (id != kNull)
		{
			Call(id, "start",std::move(matching_agent_));
		}
	}
}

void MatchActor::JoinMatch(const std::any& data)
{
	ActorId agent = std::any_cast<ActorId>(data);
	if (agent != kNull)
	{
		matching_agent_.emplace(agent,agent);
	}
}

void MatchActor::LeaveMatch(const std::any& data)
{
	ActorId agent = std::any_cast<ActorId>(data);
	if (agent != kNull)
	{
		matching_agent_.erase(agent);
	}
}

ACTOR_IMPLEMENT(MatchActor);