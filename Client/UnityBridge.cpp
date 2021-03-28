#include "UnityBridge.hpp"

#include <thread>
#include <chrono>
#include <asio.hpp>

#include "Client/UnityViewService.hpp"
#include "Client/UnityDebugService.hpp"
#include "Client/UnityInputService.hpp"
#include "Client/UnityFileService.hpp"

#include "Framework/Game/Math.hpp"
#include "Framework/Game/Game.hpp"
#include "Framework/Game/NetworkService.hpp"
#include "Framework/Game/ViewService.hpp"
#include "Framework/Game/IViewImpl.hpp"

#include "Framework/Proto/Battle.pb.h"

#ifndef _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS 1
#endif

std::unique_ptr<UnityBridge> UnityBridge::instance_;
UnityDelegate UnityBridge::unity_delegate_ = nullptr;

std::unique_ptr<Game> g_game = nullptr;
std::unique_ptr<DebugService> g_debug_service = std::make_unique<UnityDebugService>();

extern "C"
{
	EXPORT_DLL void SetUnityDelegate(UnityDelegate delegate)
	{
		UnityBridge::Get().SetExecuteDelegate(delegate);
	}

	EXPORT_DLL void InitGame(const char* data,int32_t size)
	{
		std::vector<PlayerInfo> players;
		Proto::GamePlayerInfos infos;
		infos.ParseFromArray(data, size);
		for (auto iter = infos.player_infos().cbegin(); iter < infos.player_infos().cend(); ++iter)
		{
			PlayerInfo player;
			player.id = iter->id();
			player.actor_asset = iter->actor_asset();
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
			Proto::GameCommondGroup group;
			group.ParseFromArray(data, size);
			for (auto iter = group.commonds().cbegin(); iter != group.commonds().cend(); ++iter)
			{
				
				Command cmd;
				cmd.x_axis = fixed16(std::abs(iter->second.x_axis()) < 0.1 ? 0 : iter->second.x_axis());
				cmd.y_axis = fixed16(std::abs(iter->second.y_axis()) < 0.1 ? 0 : iter->second.y_axis());
				cmd.skill = iter->second.skill();
				g_game->InputCommand(iter->first,std::move(cmd));
			}
		}
	}

	EXPORT_DLL void GameRollback()
	{
		if (g_game)
		{
			uint32_t min = 0;
			g_game->Rollback(std::max(min,g_game->run_frame() - 100));
		}
	}
}