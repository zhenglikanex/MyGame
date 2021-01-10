#pragma once

#include <vector>
#include <memory>
#include <chrono>
#include <unordered_map>
#include <cassert>

#include "Framework/Game/FileService.hpp"
#include "Framework/Game/LogService.hpp"
#include "Framework/Game/Locator.hpp"
#include "Framework/Game/System.hpp"
#include "Framework/Game/GameDefin.hpp"
#include "Framework/Game/Component/Player.hpp"
#include "Framework/Game/Component/Commond.hpp"
#include "Framework/Game/Data/Config.hpp"

#include "entt/entt.hpp"

class Snapshot
{
	
};

class Game
{
public:
	Game(Locator&& locator,GameMode mode,std::vector<Player>&& players);
	~Game();

	bool Initialize();
	void Update(float dt);
	void Finalize();
	
	void UpdateClinet(float dt);
	void UpdateServer(float dt);

	// 外部调用
	void InputCommond(uint32_t id, Commond&& commond);
	void SetupCommonds(uint32_t frame);
	CommondGroup GetCommondGroup(uint32_t frame);
	std::vector<CommondGroup> GetAllCommondGroups();

	void CheckPredict();
	CommondGroup PredictCommondGroup(uint32_t frame);

	void SaveSnapshot();

	entt::registry& registry() { return registry_; }
	uint32_t main_player_id() const { return main_player_id_; }
private:
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
	std::unordered_map<uint32_t, std::vector<Commond>> commonds_map_;
	std::unordered_map<uint32_t, CommondGroup> predict_commond_groups_;
	std::unordered_map<uint32_t, Snapshot> snapshots_;
	uint32_t main_player_id_;
};

extern std::unique_ptr<Game> g_game;