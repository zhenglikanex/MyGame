#include "UnityBridge.hpp"

#include <thread>
#include <chrono>
#include <asio.hpp>

#include "Client/UnityViewService.hpp"
#include "Client/UnityLogService.hpp"
#include "Client/UnityInputService.hpp"
#include "Client/UnityFileService.hpp"

#include "Framework/Game/Math.hpp"
#include "Framework/Game/Game.hpp"
#include "Framework/Game/NetworkService.hpp"
#include "Framework/Game/ViewService.hpp"
#include "Framework/Game/IViewImpl.hpp"

#include "Framework/Proto/Battle.pb.h"

std::unique_ptr<UnityBridge> UnityBridge::instance_;
UnityDelegate UnityBridge::unity_delegate_ = nullptr;

std::unique_ptr<Game> g_game = nullptr;
std::unique_ptr<LogService> g_log_service = std::make_unique<UnityLogService>();

extern "C"
{
	EXPORT_DLL void SetUnityDelegate(UnityDelegate delegate)
	{
		UnityBridge::Get().SetExecuteDelegate(delegate);
	}

	EXPORT_DLL void InitGame(const char* data,int32_t size)
	{
		std::vector<Player> players;
		GamePlayerInfos infos;
		infos.ParseFromArray(data, size);
		for (auto iter = infos.player_infos().cbegin(); iter < infos.player_infos().cend(); ++iter)
		{
			Player player;
			player.id = iter->id();
			player.actor_id = iter->actor_id();
			players.emplace_back(std::move(player));
		}

		Locator locator;
		locator.Set<ViewService>(std::make_unique<UnityViewService>());
		locator.Set<InputService>(std::make_unique<UnityInputService>());
		locator.Set<FileService>(std::make_unique<UnityFileService>());

		g_game = std::make_unique<Game>(std::move(locator), GameMode::kClinet,std::move(players));
		g_game->Initialize();
	}

	EXPORT_DLL void DestoryGame()
	{
		g_game = nullptr;
	}

	EXPORT_DLL void UpdateGame(float dt)
	{
		if (g_game)
		{
			g_game->Update(dt);
		}
	}

	EXPORT_DLL void GameInput(const char* data, int32_t size)
	{
		if (g_game)
		{
			GameCommondGroup group;
			group.ParseFromArray(data, size);
			for (auto iter = group.commonds().cbegin(); iter != group.commonds().cend(); ++iter)
			{
				
				Command cmd;
				cmd.x_axis = fixed16(iter->second.x_axis());
				cmd.y_axis = fixed16(iter->second.y_axis());

				g_game->InputCommond(iter->first,std::move(cmd));
			}
		}
	}
}