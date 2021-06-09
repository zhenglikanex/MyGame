#include "BattleActor.hpp"

#include <iostream>

#include "Framework/Proto/NetMessage.hpp"

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
	last_time_ = start_time;
	AddTimer(1, -1, [this]()
		{
			uint32_t now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
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
		std::cout << "---------------run_time:" << run_time_ << " frame:" << run_frame_ << std::endl;
		PushCommandGroup();
		++run_frame_;
	}
}

void BattleActor::InputCommand(const std::any& data)
{
	auto& [player, command] = std::any_cast<std::tuple<ActorId, Proto::GameCommand>>(data);
	std::cout << "Input command" << player;
	auto iter = ids_.find(player);
	if (iter == ids_.end())
	{
		return;
	}

	std::cout << " ids command" << iter->first << std::endl;

	auto id = iter->first;
	auto it = player_commands_.find(id);
	if (it == player_commands_.end())
	{
		return;
	}

	player_commands_[id].emplace_back(command);
}

void BattleActor::PushCommandGroup()
{
	Proto::GameCommandGroup group;
	group.set_frame(run_frame_);
	group.set_frame2(run_frame_);
	group.set_time(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());

	auto commands = group.mutable_commands();
	for (auto& entry : ids_)
	{
		auto it = player_commands_.find(entry.second);
		if (it != player_commands_.end())
		{
			if (it->second.size() <= run_frame_)
			{
				it->second.push_back(Proto::GameCommand());
			}
			(*commands)[entry.second] = it->second[run_frame_];
		}
	}

	std::cout << "push_command_group" << group.frame() << "  " << group.commands().size() << std::endl;
	if (group.commands().size() < 2)
	{
		assert(false && "!");
	}
	auto buffer = Serialize(group);
	std::cout << "size:" << (buffer.size()) << "   " << group.ByteSize() << std::endl;
	
	for (auto player : players_)
	{
		auto data = buffer;
		Call(player, "send", std::make_tuple(std::string("push_command_group"),std::move(data)));
	}
}

ACTOR_IMPLEMENT(BattleActor)