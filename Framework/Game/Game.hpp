#pragma once

#include <vector>
#include <memory>
#include <cstdint>

#include "Framework/Game/Locator.hpp"
#include "Framework/Game/PlayerInfo.hpp"
#include "Framework/Game/Registry.hpp"

#include "Framework/Game/System.hpp"

class GameHelper;

class Game
{
public:
	Game(std::unique_ptr<GameHelper> game_helper, Locator locator, std::vector<PlayerInfo> players);
	Game(const Game&) = delete;
	Game(Game&&) = delete;
	~Game();

	const Game& operator=(const Game&) = delete;
	const Game& operator=(Game&&) = delete;

	bool Initialize();
	void Update(uint64_t dt);
	void Finalize();
	void UpdateLogic(float dt);
	void UpdateFixedLogic();

	template <class ... Systems>
	void AddSystem()
	{
		(systems_.emplace_back(std::make_unique<Systems>(registry_)), ...);
	}

	Registry& registry() { return registry_; }
private:
	void InitializeSyncInstances();

	void SetCtx();

	void UnSetCtx();

	void LoadScene(const std::string& name);

	void CreatePlayer();

	uint64_t cur_frame_time_;
	std::unique_ptr<GameHelper> game_helper_;
	Registry registry_;
	std::vector<std::unique_ptr<System>> systems_;
	std::vector<PlayerInfo> player_infos_;
	bool running_;
};
