#include <iostream>
#include <vector>
#include <chrono>
#include "Framework/Game/Game.hpp"
#include "Framework/Game/Locator.hpp"
#include "Framework/Game/PlayerInfo.hpp"
#include "Framework/Game/Service/FileService.hpp"
#include "Framework/Proto/Battle.pb.h"
#include "Server/BattleActor/ServerFileService.hpp"
#include "Server/BattleActor/ServerGameHelper.hpp"
#include "Server/BattleActor/ServerNetworkService.hpp"
#include <memory>

#include "Framework/Game/Component/GAS/GameplayState.hpp"
#include "Server/BattleActor/ServerDebugService.hpp"


inline std::unique_ptr<DebugService> g_debug_service = std::make_unique<ServerDebugService>();
using namespace std::chrono;

std::unique_ptr<Game> CreateGame()
{
	std::vector<PlayerInfo> players;
	for (uint32_t i = 0; i < 5000; ++i)
	{
		PlayerInfo player;
		player.id = i;
		player.actor_asset = "SuperHero.json";
		players.emplace_back(std::move(player));
	}

	Locator locator;
	locator.Set<FileService>(std::make_unique<ServerFileService>());
	locator.Set<NetworkService>(std::make_unique<ServerNetworkService>(
		[](const std::string& name)
		{
			return std::optional<std::vector<uint8_t>>{};
		},
		[](uint32_t player, std::string_view name, std::vector<uint8_t>&& data)
		{

		}));

	auto game = std::make_unique<Game>(std::make_unique<ServerGameHelper>(), std::move(locator), std::move(players));
	if (!game->Initialize())
	{
		game->Finalize();
		game = nullptr;
	}

	return game;
}

int main()
{
	/*entt::registry reg;
	auto e = reg.create();
	reg.emplace<GameplayState>(e);
	std::cout << sizeof(GameplayState) << std::endl;
	GameplayState game_state;*/

	

	std::vector<std::unique_ptr<Game>> games;
	for(int i = 0;i<1;++i)
	{
		games.emplace_back(CreateGame());
	}

	

	while (true)
	{
		auto start = system_clock::now();
		/*for (int i = 0; i < 10000; ++i)
		{
			games[0]->registry().create_predict(0);
		}*/
		
		for(auto& game : games)
		{
			game->Update(GameConfig::kFrameTimeNanoseconds + 20);
		}

		auto dt = system_clock::now() - start;
		std::cout << duration_cast<milliseconds>(dt).count() << "ms" << std::endl;
	}
	
	system("pause");
	return 0;
}
