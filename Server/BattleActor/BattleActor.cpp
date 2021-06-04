#include "BattleActor.hpp"

#include <iostream>

#include "Framework/Network/NetMessage.hpp"

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

	RequestConnect("input_command", &BattleActor::InputCommand, this);

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
	
	// todo:不做上下线处理了
	for (uint32_t i = 0; i < players_.size(); ++i)
	{
		ids_.emplace(players_[i], i);
	}

	Proto::GamePlayerInfos player_infos;
	for (auto player : players_)
	{
		auto player_info = player_infos.add_player_infos();
		player_info->set_id(ids_[player]);
		player_info->set_actor_asset("hero.json");
	}

	for (auto player : players_)
	{
		Proto::StartBattleInfo info;
		info.set_my_id(ids_[player]);
		info.set_allocated_player_infos(new Proto::GamePlayerInfos(player_infos));

		Call(player, "send", std::make_tuple(std::string("start_battle"), Serialize(info)));
	}

	StartBattle();
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
		PushCommandGroup();
		++run_frame_;
	}
}

void BattleActor::InputCommand(ActorId id, const Proto::GameCommand& command)
{
	auto it = player_commands_.find(id);
	if (it == player_commands_.end())
	{
		player_commands_.emplace(id, std::vector<Proto::GameCommand>());
	}

	player_commands_[id].emplace_back(command);
}

void BattleActor::PushCommandGroup()
{
	Proto::GameCommandGroup group;
	group.set_frame(run_frame_);

	auto commands = group.mutable_commands();
	for (auto player : players_)
	{
		auto it = player_commands_.find(player);
		if (it != player_commands_.end())
		{
			if (it->second.size() <= run_frame_)
			{
				it->second.push_back(Proto::GameCommand());
			}
			(*commands)[player] = it->second[run_frame_];
		}
	}

	auto buffer = Serialize(group);
	
	for (auto player : players_)
	{
		auto data = buffer;
		Call(player, "send", std::make_tuple(std::string("push_command_group"),std::move(data)));
	}
}

ACTOR_IMPLEMENT(BattleActor)