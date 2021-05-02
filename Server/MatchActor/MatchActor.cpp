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

	RequestConnect("join_match", &MatchActor::JoinMatch, this);
	RequestConnect("leave_match", &MatchActor::LeaveMatch, this);

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
		// todo ����ս������
		auto id = StartActor("BattleActor.dll");
		if (id != kNull)
		{
			Call(id, "start", std::make_any<decltype(matching_agent_)>(matching_agent_));

			for (auto& entry : matching_agent_)
			{
				Call(entry.second, "join_battle", std::make_any<ActorId>(entry.second));
			}
			matching_agent_.clear();
		}
	}
}

bool MatchActor::JoinMatch(const std::any& data)
{
	ActorId agent = std::any_cast<ActorId>(data);
	if (agent != kNull)
	{
		matching_agent_.emplace(agent,agent);
		return true;
	}
	return false;
}

bool MatchActor::LeaveMatch(const std::any& data)
{
	ActorId agent = std::any_cast<ActorId>(data);
	if (agent != kNull)
	{
		matching_agent_.erase(agent);
		return true;
	}

	return false;
}

ACTOR_IMPLEMENT(MatchActor);