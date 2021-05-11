#include "BattleActor.hpp"
#include <iostream>

#include "Framework/Proto/Battle.pb.h"

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
	for (uint32_t i = 0; i < players_.size(); ++i)
	{
		Proto::StartBattleInfo info;
		info.set_my_id(i);
		auto player_infos = info.mutable_player_infos();
		for (uint32_t j = 0; j < players_.size(); ++j)
		{
			auto player = player_infos->add_player_infos();
			player->set_id(j);
			player->set_actor_asset("hero.json");
		}

		Call(players_[i], "send", std::move(info));
	}
}


ACTOR_IMPLEMENT(BattleActor)