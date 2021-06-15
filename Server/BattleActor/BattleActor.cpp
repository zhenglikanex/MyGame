#include "BattleActor.hpp"

#include <iostream>

#include "Framework/Proto/NetMessage.hpp"

using namespace std::chrono;

BattleActor::BattleActor(ActorId id)
	: Actor(id)
	, start_(false)
	, start_time_(0)
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

	ActorConnect("start_battle", &BattleActor::StartBattle, this);
	ActorConnect("input_command", &BattleActor::InputCommand, this);

	return true;
}

void BattleActor::Stop()
{

}

void BattleActor::Receive(ActorMessage&& actor_msg)
{
}

void BattleActor::StartBattle(const std::any& data)
{
	players_ = std::any_cast<std::vector<ActorId>>(data);
	std::cout << "Battle Start Success!" << std::endl;
	
	// todo:不做上下线处理了
	for (uint32_t i = 0; i < players_.size(); ++i)
	{
		ids_.emplace(players_[i], i);
		player_commands_.emplace(i, std::vector<Proto::GameCommand>());
	}

	Proto::GamePlayerInfos player_infos;
	for (auto player : players_)
	{
		auto player_info = player_infos.add_player_infos();
		player_info->set_id(ids_[player]);
		player_info->set_actor_asset("hero.json");
	}

	auto start_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	for (auto player : players_)
	{
		Proto::StartBattleInfo info;
		info.set_my_id(ids_[player]);
		info.set_allocated_player_infos(new Proto::GamePlayerInfos(player_infos));
		info.set_start_time(start_time);

		Call(player, "send", std::make_tuple(std::string("start_battle"), Serialize(info)));
	}

	start_ = true;
	start_time_ = start_time;
	AddTimer(1, -1, [this]()
		{
			Update();
		});

	std::cout << "BattleActor start!!!" << std::endl;
}

void BattleActor::Update()
{
	// todo：计时方式后面修改
	uint32_t now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	run_time_ = (now - start_time_) / 1000.0f;
	while (run_time_ > run_frame_ * kFrameTime + kFrameTime)
	{
		PushCommandGroup();
		++run_frame_;
	}
}

void BattleActor::InputCommand(const std::any& data)
{
	auto& [player, command] = std::any_cast<std::tuple<ActorId, Proto::GameCommand>>(data);
	
	auto iter = ids_.find(player);
	if (iter == ids_.end())
	{
		return;
	}

	auto id = iter->second;
	auto it = player_commands_.find(id);
	if (it == player_commands_.end())
	{
		return;
	}

	std::cout << "command frame " << command.frame() << "cur frame " << player_commands_[id].size() << "    " << run_frame_ << std::endl;

	if (player_commands_[id].size() <= command.frame())
	{
		player_commands_[id].emplace_back(command);
	}
}

void BattleActor::PushCommandGroup()
{
	Proto::GameCommandGroup group;
	group.set_frame(run_frame_);

	auto commands = group.mutable_commands();
	for (auto& entry : ids_)
	{
		auto it = player_commands_.find(entry.second);
		if (it != player_commands_.end())
		{
			while (run_frame_ >= it->second.size())
			{
				if (it->second.empty())
				{
					it->second.push_back(Proto::GameCommand());
				}
				else
				{
					it->second.push_back(it->second.back());
				}
				
			}
			(*commands)[entry.second] = it->second[run_frame_];
		}
	}

	assert(group.commands().size() >= players_.size());

	auto buffer = Serialize(group);	
	for (auto player : players_)
	{
		auto data = buffer;
		Call(player, "send", std::make_tuple(std::string("push_command_group"),std::move(data)));
	}
}

ACTOR_IMPLEMENT(BattleActor)