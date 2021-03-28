#pragma once

#include <vector>
#include <memory>
#include <chrono>
#include <unordered_map>
#include <cassert>
#include <cstdint>
#include <queue>

#include "Framework/Game/GameConfig.hpp"
#include "Framework/Game/FileService.hpp"
#include "Framework/Game/DebugService.hpp"
#include "Framework/Game/Locator.hpp"
#include "Framework/Game/PlayerInfo.hpp"
#include "Framework/Game/RingArray.hpp"

#include "Framework/Game/Component/Command.hpp"

#include "Framework/Game/System.hpp"

#include "Framework/Game/Data/Config.hpp"

#include "Kanex.hpp"

#include "3rdparty/include/entt/entt.hpp"

struct Snapshot
{
	uint32_t frame;
	kanex::Buffer buffer{1024};
};

enum class GameMode
{
	kClinet,
	kServer
};

using CommandGroupO = std::unordered_map<uint32_t, Command>;

struct CommandGroup
{
	uint32_t frame;
	std::unordered_map<uint32_t, Command> commands;

	CommandGroup()
		: frame(std::numeric_limits<uint32_t>::max())
	{

	}
};

class Game
{
public:
	Game(Locator&& locator,GameMode mode,std::vector<PlayerInfo>&& players);
	~Game();

	bool Initialize();
	void Update(float dt);
	void Finalize();
	
	void UpdateInput();
	void UpdateClinet();
	void UpdateServer();

	// 外部调用
	void InputCommand(uint32_t id, Command&& command);
	void SetupCommands(uint32_t frame);

	void SaveSnapshot();
	void Rollback(uint32_t frame);
	
	uint32_t run_frame() { return run_frame_; }
	entt::registry& registry() { return registry_; }
private:
	void LoadAllConfig();
	void CreatePlayer();

	template<class T>
	std::enable_if_t<std::is_base_of_v<BaseConfig,T>,void>
		LoadConfig(const std::string& file)
	{
		auto& locator = registry_.ctx<Locator>();
		auto& file_service = locator.Ref<const FileService>();
		try
		{
			T config;
			std::string data = file_service.OpenFile(file);
			config.Load(data);
			registry_.set<T>(std::move(config));
		}
		catch (std::exception& e)
		{
			ERROR("{} load failed,error:{}", file, e.what());
			//assert(false && "load config failed!");
		}
	}

	GameMode game_mode_;
	float run_time_;		// 注意,这个浮点数不会引起不同步的问题
	uint32_t run_frame_;
	uint32_t real_frame_;
	float input_frame_rate_;	// 注意,这个浮点数不会引起不同步的问题
	
	entt::registry registry_;
	std::vector<std::unique_ptr<System>> systems_;
	std::vector<PlayerInfo> player_infos_;
	std::array<CommandGroup, GameConfig::kMaxPredictFrame> command_groups_;
	std::array<Snapshot,GameConfig::kMaxPredictFrame> snapshots_;
};

extern std::unique_ptr<Game> g_game;