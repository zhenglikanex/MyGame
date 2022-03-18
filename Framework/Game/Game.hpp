#pragma once

#include <vector>
#include <memory>
#include <chrono>
#include <unordered_map>
#include <cassert>
#include <cstdint>

#include "Framework/Game/GameConfig.hpp"
#include "Framework/Game/Locator.hpp"
#include "Framework/Game/PlayerInfo.hpp"
#include "Framework/Game/Registry.hpp"

#include "Framework/Game/Component/Command.hpp"

#include "Framework/Game/System.hpp"

#include "Kanex.hpp"

class Game
{
public:
	Game(Locator&& locator,std::vector<PlayerInfo>&& players);
	~Game();

	bool Initialize(uint32_t start_time);
	void Update(float dt);
	void Finalize();
	
	void UpdateInput();
	void UpdateLogic(float dt);
	void UpdateFixedLogic();

	void InputCommand(uint32_t frame,uint32_t id,const Command& command);
	uint32_t GetVerifiedFrame() const;

	uint32_t start_time() const { return start_time_; }
	float run_time() const { return run_time_; }
	uint32_t run_frame() const { return run_frame_; }
	Registry& registry() { return registry_; }
private:
	void InitializeSyncInstances();
	void SetCtx();
	void UnSetCtx();
	void LoadScene(const std::string& name);
	void CreatePlayer();

	CommandGroup& GetCommandGroup(uint32_t frame);

	uint32_t start_time_;
	float run_time_;
	uint32_t run_frame_;
	float input_frame_rate_;
	Registry registry_;
	std::vector<std::unique_ptr<System>> systems_;
	std::vector<PlayerInfo> player_infos_;
	std::array<CommandGroup, GameConfig::kMaxPredictFrame> command_groups_;
};

extern std::unique_ptr<Game> g_game;