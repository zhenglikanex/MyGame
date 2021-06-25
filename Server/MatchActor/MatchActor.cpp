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

	std::cout << "MatchInit:" << id() << std::endl;

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
		std::cout << "match" << std::endl;
		// todo ����ս������
		auto id = StartActor("BattleActor.dll");
		if (id != kNull)
		{
			for (auto& entry : matching_agent_)
			{
				Call(entry.first, "join_battle", id);
			}

			std::vector<ActorId> players;
			for (auto& entry : matching_agent_)
			{
				players.emplace_back(entry.first);
			}
			Call(id, "start_battle", players);
		}

		matching_agent_.clear();
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