#include "UnityBridge.hpp"

#include "Client/UnityViewService.hpp"

#include <thread>
#include <chrono>

#include <asio.hpp>

#include "Framework/Game/Game.hpp"
#include "Framework/Game/NetworkService.hpp"
#include "Framework/Game/ViewService.hpp"
#include "Framework/Game/IViewImpl.hpp"

#include "Framework/Proto/Battle.pb.h"

std::unique_ptr<UnityBridge> UnityBridge::instance_;
UnityDelegate UnityBridge::unity_delegate_ = nullptr;

std::unique_ptr<Game> g_game = nullptr;

extern "C"
{
	EXPROT_DLL void SetUnityDelegate(UnityDelegate delegate)
	{
		UnityBridge::Get().SetExecuteDelegate(delegate);
	}

	EXPROT_DLL void InitGame(const char* data,int32_t size)
	{
		std::vector<Player> players;
		GamePlayerInfos infos;
		infos.ParseFromArray(data, size);
		for (auto iter = infos.player_infos().cbegin(); iter < infos.player_infos().cend(); ++iter)
		{
			Player player;
			player.id = iter->id();
			player.actor_id = iter->actor_id();
		}
		Locator locator;
		locator.Set<ViewService>(std::make_shared<UnityViewService>());
		g_game = std::make_unique<Game>(std::move(locator), GameMode::kClinet,std::move(players));
		g_game->Initialize();
	}

	EXPROT_DLL void DestoryGame()
	{
		g_game = nullptr;
	}

	EXPROT_DLL void UpdateGame(float dt)
	{
		if (g_game)
		{
			g_game->Update(dt);
		}
	}
}