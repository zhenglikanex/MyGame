#pragma once

#include <vector>
#include <memory>
#include <chrono>
#include <unordered_map>
#include <cassert>

#include "Framework/Game/FileService.hpp"
#include "Framework/Game/DebugService.hpp"
#include "Framework/Game/Locator.hpp"
#include "Framework/Game/PlayerInfo.hpp"

#include "Framework/Game/Component/Command.hpp"

#include "Framework/Game/System.hpp"

#include "Framework/Game/Data/Config.hpp"

#include "entt/entt.hpp"

class Snapshot
{
	
};

enum class GameMode
{
	kClinet,
	kServer
};

using CommandGroup = std::unordered_map<uint32_t, Command>;

class Game
{
public:
	Game(Locator&& locator,GameMode mode,std::vector<PlayerInfo>&& players);
	~Game();

	bool Initialize();
	void Update(float dt);
	void Finalize();
	
	void UpdateClinet(float dt);
	void UpdateServer(float dt);

	// 外部调用
	void InputCommand(uint32_t id, Command&& command);
	void SetupCommands(uint32_t frame);
	Command PredictCommand(uint32_t id);

	void CheckPredict();
	void SaveSnapshot();

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

	static const uint32_t kFrameRate = 33;			// 33毫秒更新频率
	static const uint32_t kMaxPredictFrame = 60;	//最大预测60帧,2秒

	entt::registry registry_;
	std::vector<std::unique_ptr<System>> systems_;
	std::vector<PlayerInfo> player_infos_;
	std::unordered_map<uint32_t, std::vector<Command>> player_input_commands_;	// 玩家的输入队列
	std::vector<CommandGroup> command_groups_;
	std::unordered_map<uint32_t, CommandGroup> predict_command_groups_;
	std::unordered_map<uint32_t, Snapshot> snapshots_;
};

extern std::unique_ptr<Game> g_game;