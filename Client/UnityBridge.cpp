#include "UnityBridge.hpp"

#include <thread>
#include <chrono>
#include <asio.hpp>

#include "Client/UnityViewService.hpp"
#include "Client/UnityDebugService.hpp"
#include "Client/UnityInputService.hpp"
#include "Client/UnityFileService.hpp"
#include "Client/ClientNetworkService.hpp"

#include "Framework/Game/Math.hpp"
#include "Framework/Game/Game.hpp"
#include "Framework/Game/NetworkService.hpp"
#include "Framework/Game/ViewService.hpp"
#include "Framework/Game/IViewImpl.hpp"

#include "Framework/Proto/Battle.pb.h"

#ifndef _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS 1
#endif

using namespace std::chrono;

std::unique_ptr<UnityBridge> UnityBridge::instance_;
UnityDelegate UnityBridge::unity_delegate_ = nullptr;

std::unique_ptr<Game> g_game = nullptr;
std::unique_ptr<DebugService> g_debug_service = std::make_unique<UnityDebugService>();
std::unique_ptr<ClientNetworkService> g_network_service = nullptr;
uint32_t g_my_id = 0;
uint32_t g_ping = 0;

extern "C"
{
	EXPORT_DLL void SetUnityDelegate(UnityDelegate delegate)
	{
		UnityBridge::Get().SetExecuteDelegate(delegate);
	}

	EXPORT_DLL void InitGame(const Proto::GamePlayerInfos& infos)
	{
		std::vector<PlayerInfo> players;
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
		g_network_service = nullptr;
		g_game = nullptr;
	}

	EXPORT_DLL void UpdateGame(float dt)
	{
		if (g_network_service)
		{
			if (g_network_service->IsConnected())
			{
				g_network_service->Update();
			}
			else
			{
				auto code = g_network_service->GetLastError();
				return;
			}
		}

		CheckPing();

		if (g_game)
		{
			g_game->Update(dt);
		}
	}

	EXPORT_DLL void GameInput(const char* data, int32_t size)
	{
		if (g_game)
		{
			Proto::CS2CPlusPlusCommand csharp_command;
			csharp_command.ParseFromArray(data, size);

			Command command;
			command.x_axis = fixed16(std::abs(csharp_command.x_axis()) < 0.1 ? 0 : csharp_command.x_axis());
			command.y_axis = fixed16(std::abs(csharp_command.y_axis()) < 0.1 ? 0 : csharp_command.y_axis());
			command.skill = csharp_command.skill();
			command.jump = csharp_command.jump();

			g_game->InputCommand(g_game->run_frame(),g_my_id, command);

			Proto::GameCommand game_command;
			game_command.set_x_axis(command.x_axis.raw_value());
			game_command.set_y_axis(command.y_axis.raw_value());
			game_command.set_skill(command.skill);
			game_command.set_jump(command.jump);
			

			if (g_network_service)
			{
				g_network_service->Send("input_command", Serialize(game_command));
			}
		}
	}

	EXPORT_DLL void GameRollback()
	{
		if (g_game)
		{
			uint32_t min = 0;
			g_game->Rollback(std::max(min,g_game->run_frame() - 1000));
		}
	}

	EXPORT_DLL void JoinMatch()
	{
		// 偷懒了匹配本来想放在unity端做的,这里简单的请求下得了
		if (!g_network_service)
		{
			g_network_service = std::make_unique<ClientNetworkService>();
		}
		
		if (g_network_service->Connect("127.0.0.1", 9523, 500))
		{
			// 偷懒写这里了
			g_network_service->set_messge_handler([](const NetMessage& message)
				{
					if (message.name() == "start_battle")
					{
						INFO("start_battle");

						Proto::StartBattleInfo info;
						info.ParseFromArray(message.data().data(), message.data().size());
						UnityBridge::Get().CallUnity<void>("SetMyId", info.my_id());
						InitGame(info.player_infos());
					}
					else if (message.name() == "push_command_group")
					{
						if (!g_game)
						{
							return;
						}

						Proto::GameCommandGroup game_command_group;
						game_command_group.ParseFromArray(message.data().data(), message.data().size());

						if (game_command_group.commands().size() < 2)
						{
							INFO("!");
						}

						CommandGroup group;
						group.frame = game_command_group.frame();
						for (auto& entry : game_command_group.commands())
						{
							Command command;
							command.x_axis = entry.second.x_axis();
							command.y_axis = entry.second.y_axis();
							command.skill = entry.second.skill();
							command.jump = entry.second.jump();
							
							if (entry.first != g_my_id)
							{
								INFO("x{} y{}", entry.second.x_axis(), entry.second.y_axis());
							}

							group.commands.emplace(entry.first, command);
						}

						if (group.frame >= g_game->run_frame())
						{
							INFO("server group frame{} real_frame{} run_frame{}", group.frame, g_game->real_frame(),g_game->run_frame());
							for (auto& entry : group.commands)
							{
								g_game->InputCommand(group.frame, entry.first, entry.second);
							}
						}
						else
						{
							INFO("---------------group frame{} real_frame{} {}", group.frame, g_game->real_frame(),g_game->run_frame());
							assert(group.frame == g_game->real_frame());

							if (!g_game->CheckPredict(group))
							{
								g_game->FixPredict(g_my_id, group);
							}

							g_game->set_real_frame(group.frame);
						}
					}
					else if (message.name() == "push_command")
					{

					}
					else if (message.name() == "ping")
					{
						
						Proto::Ping ping;
						ping.ParseFromArray(message.data().data(), message.data().size());

						uint32_t now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
						g_ping = now - ping.time();

						//INFO("ping:{}", g_ping);
					}
				});

			g_network_service->Request("join_match", std::vector<uint8_t>{}, [](const std::vector<uint8_t>& buffer)
				{
					INFO("join_match!")
					// 偷懒不处理了
				});
		}
	}

	EXPORT_DLL void CheckPing()
	{
		if (g_network_service)
		{
			uint32_t time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

			Proto::Ping ping;
			ping.set_time(time);

			g_network_service->Send("ping",Serialize(ping));
		}
	}
}
 