#include "BattleActor.hpp"

#include <functional>

#include "Framework/Proto/NetMessage.hpp"

#include "Server/BattleActor/ServerFileService.hpp"
#include "Server/BattleActor/ServerGameHelper.hpp"
#include "Server/BattleActor/ServerNetworkService.hpp"

#include "Framework/Proto/Battle.pb.h"

#include "Kanex.hpp"

using namespace std::chrono;

BattleActor::BattleActor(ActorId id)
	: Actor(id)
	, start_(false)
	, last_time_(0)
{

}

bool BattleActor::Init(const std::shared_ptr<ActorNet>& actor_net)
{
	if (!Actor::Init(actor_net))
	{
		return false;
	}

	ActorConnect("start_battle", &BattleActor::StartBattle, this);
	ActorConnect("push_game_message", &BattleActor::PushGameMessage, this);

	return true;
}

void BattleActor::Stop()
{
	if (game_)
	{
		game_->Finalize();
		game_ = nullptr;
	}
}

void BattleActor::Receive(ActorMessage&& actor_msg)
{

}

void BattleActor::StartBattle(const std::any& data)
{
	players_ = std::any_cast<std::vector<ActorId>>(data);
	std::cout << "Battle Start Success!" << std::endl;
	
	// todo:不做上下线处理了
	Proto::GamePlayerInfos player_infos;
	for (auto player : players_)
	{
		auto player_info = player_infos.add_player_infos();
		player_info->set_id(ids_[player]);
		player_info->set_actor_asset("SuperHero.json");
	}

	auto start_time = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch()).count();
	for (auto player : players_)
	{
		Proto::StartBattleInfo info;
		info.set_my_id(ids_[player]);

		auto player_infos = info.mutable_player_infos();
		auto player_info = player_infos->add_player_infos();
		player_info->set_id(ids_[player]);
		player_info->set_actor_asset("SuperHero.json");

		info.set_start_time(start_time);

		Call(player, "send", std::make_tuple(std::string("start_battle"), Serialize(info)));
	}

	start_ = true;
	last_time_ = start_time;
	
	std::vector<PlayerInfo> players;
	for (auto iter = player_infos.player_infos().cbegin(); iter < player_infos.player_infos().cend(); ++iter)
	{
		PlayerInfo player;
		player.id = iter->id();
		player.actor_asset = iter->actor_asset();
		players.emplace_back(std::move(player));
	}

	Locator locator;
	locator.Set<FileService>(std::make_unique<ServerFileService>());
	locator.Set<NetworkService>(std::make_unique<ServerNetworkService>(
		std::bind(&BattleActor::RecvGameMessage, this, std::placeholders::_1),
		std::bind(&BattleActor::SendGameMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
	
	game_ = std::make_unique<Game>(std::make_unique<ServerGameHelper>(),std::move(locator),std::move(players));
	if (!game_->Initialize())
	{
		game_->Finalize();
		game_ = nullptr;
	}

	AddTimer(1, -1, [this]()
		{
			Update();	
		});
}

void BattleActor::Update()
{
	if (game_)
	{
		uint64_t now = duration_cast<nanoseconds>(high_resolution_clock::now().time_since_epoch()).count();
		game_->Update(now - last_time_);
		last_time_ = now;
	}
}

void BattleActor::PushGameMessage(const std::any& data)
{
	auto& [name, message_data] = std::any_cast<std::tuple<std::string, std::vector<uint8_t>>&>(data);
	
	if (game_messages_.find(name) == game_messages_.end())
	{
		game_messages_.emplace(name, std::queue<std::vector<uint8_t>>());
	}
	
	auto& messages = game_messages_[name];
	messages.emplace(std::move(message_data));
}

std::optional<std::vector<uint8_t>> BattleActor::RecvGameMessage(const std::string & name)
{
	if (game_messages_.find(name) == game_messages_.end())
	{
		return {};
	}

	auto& messages = game_messages_[name];

	if (messages.empty())
	{
		return {};
	}

	auto message = std::move(messages.front());
	messages.pop();
	return message;
}

void BattleActor::SendGameMessage(uint32_t player,std::string_view name, std::vector<uint8_t>&& data)
{
	for (auto& item : ids_)
	{
		if (player == item.second)
		{
			Call(item.first, "send", std::make_tuple(std::string(name.data()), data));
		}
	}
}

ACTOR_IMPLEMENT(BattleActor)
