#pragma once

#include <vector>
#include <memory>
#include <chrono>
#include <unordered_map>

#include "Framework/Game/Locator.hpp"
#include "Framework/Game/System.hpp"
#include "Framework/Game/GameDefin.hpp"
#include "Framework/Game/FixedPoint.hpp"
#include "Framework/Game/Component/Player.hpp"
#include "Framework/Game/Component/CommondList.hpp"

#include "entt/entt.hpp"

class Snapshot
{
	
};

class Game
{
public:
	Game(Locator&& locator,GameMode mode);
	~Game();

	bool Initialize();
	void Update(float dt);
	void Finalize();
	
	// 外部调用
	void InputCommond(uint32_t id, Commond&& commond);
	void SetupCommonds(uint32_t frame);
	CommondGroup GetCommondGroup(uint32_t frame);
	std::vector<CommondGroup> GetAllCommondGroups();

	CommondGroup PredictCommondGroup(uint32_t frame);
	void CheckPredictFrame();

	void SaveSnapshot();
private:
	static const uint32_t kFrameRate = 33;	// 33毫秒更新频率
	static const uint32_t kMaxPredictFrame = 60;	//最大预测60帧,2秒

	entt::registry registry_;
	std::vector<std::unique_ptr<System>> systems_;
	std::unordered_map<uint32_t, std::vector<Commond>> commonds_map_;
	std::unordered_map<uint32_t, CommondGroup> predict_commond_groups_;
	std::unordered_map<uint32_t, Snapshot> snapshots_;
};