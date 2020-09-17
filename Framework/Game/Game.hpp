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
	
	void UpdateClinet(float dt);
	void UpdateServer(float dt);

	// �ⲿ����
	void InputCommond(uint32_t id, Commond&& commond);
	void SetupCommonds(uint32_t frame);
	CommondGroup GetCommondGroup(uint32_t frame);
	std::vector<CommondGroup> GetAllCommondGroups();

	void CheckPredict();
	CommondGroup Predict(uint32_t frame);
	CommondGroup PredictCommondGroup(uint32_t frame);

	void SaveSnapshot();
private:
	static const uint32_t kFrameRate = 33;	// 33�������Ƶ��
	static const uint32_t kMaxPredictFrame = 60;	//���Ԥ��60֡,2��

	entt::registry registry_;
	std::vector<std::unique_ptr<System>> systems_;
	std::unordered_map<uint32_t, std::vector<Commond>> commonds_map_;
	std::unordered_map<uint32_t, CommondGroup> predict_commond_groups_;
	std::unordered_map<uint32_t, Snapshot> snapshots_;
};