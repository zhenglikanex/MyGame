#include "BattleActor.hpp"
#include <iostream>

using namespace std::chrono;

BattleActor::BattleActor(ActorId id)
	: Actor(id)
	, start_(false)
	, last_time_(0)
	, time_(0)
	, frame_(0)
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

	RequestConnect("start_battle", &BattleActor::StartBattle, this);

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

void BattleActor::StartBattle()
{
	if (start_)
	{
		return;
	}

	start_ = true;
	AddTimer(1, -1, [this]()
		{

			last_time_ = system_clock::now().time_since_epoch().count();
		});

	std::cout << "BattleActor start!!!" << std::endl;
}

void BattleActor::Update(uint32_t dt)
{

}

void BattleActor::InputPlayerCommand(ActorId id, const Proto::GameCommond& command)
{

}

void BattleActor::PushPlayerCommand()
{
	for (auto player : players_)
	{
		Proto::GameCommondGroup group;
		Call(player, "send", std::move(group));
	}
}

ACTOR_IMPLEMENT(BattleActor)