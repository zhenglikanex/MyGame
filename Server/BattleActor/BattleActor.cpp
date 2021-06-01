#include "BattleActor.hpp"
#include <iostream>

using namespace std::chrono;

BattleActor::BattleActor(ActorId id)
	: Actor(id)
	, start_(false)
	, last_time_(0)
	, run_time_(0)
	, run_frame_(0)
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
	last_time_ = system_clock::now().time_since_epoch().count();
	AddTimer(1, -1, [this]()
	{
		uint32_t now = system_clock::now().time_since_epoch().count();
		Update((now - last_time_) / 1000.f);
		last_time_ = now;
	});

	std::cout << "BattleActor start!!!" << std::endl;
}

void BattleActor::Update(float dt)
{
	run_time_ += dt;
	while (run_time_ > run_frame_ * kFrameTime + kFrameTime)
	{
		PushPlayerCommand();
		++run_frame_;
	}
}

void BattleActor::InputPlayerCommand(ActorId id, const Proto::GameCommond& command)
{
	auto it = player_commands_.find(id);
	if (it == player_commands_.end())
	{
		player_commands_.emplace(id, std::vector<Proto::GameCommond>());
	}

	player_commands_[id].emplace_back(command);
}

void BattleActor::PushPlayerCommand()
{
	Proto::GameCommondGroup group;
	auto commands = group.mutable_commonds();
	for (auto player : players_)
	{
		auto it = player_commands_.find(player);
		if (it != player_commands_.end())
		{
			if (it->second.size() <= run_frame_)
			{
				it->second.push_back(Proto::GameCommond());
			}
			
			
		}
	}

	for (auto player : players_)
	{
		
		Call(player, "send", std::move(group));
	}
}

ACTOR_IMPLEMENT(BattleActor)