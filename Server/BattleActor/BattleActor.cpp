#include "BattleActor.hpp"

#include <functional>

#include "Framework/Proto/NetMessage.hpp"

#include "Server/BattleActor/ServerFileService.hpp"
#include "Server/BattleActor/ServerInputService.hpp"
#include "Server/BattleActor/ServerViewService.hpp"
#include "Server/BattleActor/ServerGameHelper.hpp"

using namespace std::chrono;

BattleActor::BattleActor(ActorId id)
	: Actor(id)
	, start_(false)
	, start_time_(0)
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
	if (g_game)
	{
		g_game->Finalize();
		g_game = nullptr;
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
	
	std::vector<PlayerInfo> players;
	for (auto iter = player_infos.player_infos().cbegin(); iter < player_infos.player_infos().cend(); ++iter)
	{
		PlayerInfo player;
		player.id = iter->id();
		player.actor_asset = iter->actor_asset();
		players.emplace_back(std::move(player));
	}

	auto input_service = std::make_unique<ServerInputService>();
	input_service->set_input_handler(std::bind(&BattleActor::GameInput,this));

	Locator locator;
	locator.Set<ViewService>(std::make_unique<ServerViewService>());
	locator.Set<InputService>(std::move(input_service));
	locator.Set<FileService>(std::make_unique<ServerFileService>());
	locator.Set<ServerGameHelper>(std::make_unique<ServerGameHelper>());
	
	g_game = std::make_unique<Game>(std::move(locator),std::move(players));
	g_game->Initialize();
	g_game->set_start_time(start_time);

	AddTimer(1, -1, [this]()
		{
			Update();
		});
}

void BattleActor::Update()
{
	g_game->Update(0);
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

	if (player_commands_[id].size() <= command.frame())
	{
		player_commands_[id].emplace_back(command);
	}
}

std::vector<std::unique_ptr<System>> BattleActor::CreateSystems()
{
	std::vector<std::unique_ptr<System>> systems;
	systems.emplace_back(std::make_unique<CreateActorSystem>(registry_));
	//	systems_.emplace_back(std::make_unique<CreateViewSystem>(registry_));
	//	systems_.emplace_back(std::make_unique<CreateAnimationSystem>(registry_));
	//	systems_.emplace_back(std::make_unique<CreateSkillGraphSystem>(registry_));
	//	systems_.emplace_back(std::make_unique<HealthSystem>(registry_));
	//	systems_.emplace_back(std::make_unique<ModifyHealthSystem>(registry_));
	//	systems_.emplace_back(std::make_unique<ActorStateSystem>(registry_));
	//	systems_.emplace_back(std::make_unique<SkillStateSystem>(registry_));
	//	systems_.emplace_back(std::make_unique<RootMotionSystem>(registry_));
	//	systems_.emplace_back(std::make_unique<MovementSystem>(registry_));
	//	systems_.emplace_back(std::make_unique<SkillSystem>(registry_));
	//	systems_.emplace_back(std::make_unique<AnimationSystem>(registry_));
	//	systems_.emplace_back(std::make_unique<UpdateColliderTransformSystem>(registry_));
	//	systems_.emplace_back(std::make_unique<CollisionSystem>(registry_));
	//	systems_.emplace_back(std::make_unique<UpdateViewSystem>(registry_));
	//
	//#ifdef DEBUG
	//	systems_.emplace_back(std::make_unique<DebugSystem>(registry_));
	//#endif
}

void BattleActor::GameInput() const
{

}

ACTOR_IMPLEMENT(BattleActor)