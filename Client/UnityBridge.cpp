#include "UnityBridge.hpp"

#include <thread>
#include <chrono>
#include <queue>
#include <asio.hpp>

#include "Client/ClientNetwork.hpp"
#include "Client/ClientNetworkService.hpp"
#include "Client/ClientGameHelper.hpp"
#include "Client/UnityViewService.hpp"
#include "Client/UnityDebugService.hpp"
#include "Client/UnityInputService.hpp"
#include "Client/UnityFileService.hpp"
#include "Client/UnityUIService.hpp"

#include "Framework/Game/Math.hpp"
#include "Framework/Game/Game.hpp"

#include "Framework/Proto/Battle.pb.h"

#ifndef _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS 1
#endif

using namespace std::chrono;

std::unique_ptr<UnityBridge> UnityBridge::instance_;
UnityDelegate UnityBridge::unity_delegate_ = nullptr;

std::unique_ptr<Game> g_game = nullptr;
std::unique_ptr<DebugService> g_debug_service = std::make_unique<UnityDebugService>();
std::unique_ptr<ClientNetwork> g_network_service = nullptr;

uint32_t g_my_id = 0;
uint32_t g_ping = 0;
uint64_t g_last_time = 0;

std::unordered_map<std::string,std::queue<NetMessage>> g_game_messages;

std::optional<NetMessage> RecvGameMessage(const std::string& name)
{
	if (g_game_messages.find(name) == g_game_messages.end())
	{
		return {};
	}

	auto& messages = g_game_messages[name];

	if (messages.empty())
	{
		return {};
	}

	auto message = std::move(messages.front());
	messages.pop();
	return message;
}

void SendGameMessage(std::string_view name,std::vector<uint8_t>&& data)
{
	if (g_network_service)
	{
		g_network_service->Send(name, std::move(data));
	}
}

void InitGame(const Proto::GamePlayerInfos& infos, uint32_t start_time,uint32_t local_id)
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

	locator.Set<FileService>(std::make_unique<UnityFileService>());
	auto& file_service = locator.Ref<FileService>();
	file_service.set_cur_path(UnityBridge::Get().CallUnity<std::string>("GetDataPath") + "/");

	locator.Set<ViewService>(std::make_unique<UnityViewService>());
	locator.Set<InputService>(std::make_unique<UnityInputService>());
	locator.Set<UIService>(std::make_unique<UnityUIService>());
	locator.Set<NetworkService>(std::make_unique<ClientNetworkService>(&RecvGameMessage, &SendGameMessage));

	g_game = std::make_unique<Game>(std::make_unique<ClientGameHelper>(local_id),std::move(locator),std::move(players));
	if (!g_game->Initialize())
	{
		g_game->Finalize();
		g_game = nullptr;
	}

	if (g_game)
	{
		//加上服务器到这里的延迟
		uint32_t delay = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch()).count() - start_time;

		//让客户端领先服务器1帧加半个rtt保证，使客户端的输入能在服务器执行当前帧时收到，后面可以再根据ping值来调整客户端领先的值
		// todo:这里计算的g_ping有点不太准,暂时放大领先的帧数
		g_game->Update((delay + g_ping / 1.5 + 34) * 1000000);
	}
}

void CheckPing()
{
	if (g_network_service)
	{
		uint32_t time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

		Proto::Ping ping;
		ping.set_time(time);

		g_network_service->Send("ping", Serialize(ping));
	}
}

extern "C"
{
	EXPORT_DLL void SetUnityDelegate(UnityDelegate delegate)
	{
		UnityBridge::SetExecuteDelegate(delegate);
	}

	
	EXPORT_DLL void DestoryGame()
	{
		g_network_service = nullptr;
		if (g_game)
		{
			g_game->Finalize();
			g_game = nullptr;
		}
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
				INFO("network_error {} ", (int)code);
				return;
			}
		}

		CheckPing();

		if (g_game)
		{
			try
			{
				uint64_t now = duration_cast<nanoseconds>(high_resolution_clock::now().time_since_epoch()).count();
				g_game->Update(now - g_last_time);
				g_last_time = now;
			}
			catch (std::exception& exception)
			{
				INFO("exception : {}", exception.what());
			}
			catch (...)
			{
				//assert(false);
			}
		}
	}

	EXPORT_DLL void JoinMatch()
	{
		// 偷懒了匹配本来想放在unity端做的,这里简单的请求下得了
		if (!g_network_service)
		{
			g_network_service = std::make_unique<ClientNetwork>();
		}
		
		if (g_network_service->Connect("127.0.0.1", 9523, 500))
		{
			// 偷懒写这里了
			g_network_service->set_messge_handler([](NetMessage&& message)
				{
				
					if (message.name() == "start_battle")
					{
						INFO("start_battle");
						
						g_last_time = duration_cast<nanoseconds>(high_resolution_clock::now().time_since_epoch()).count();

						Proto::StartBattleInfo info;
						info.ParseFromArray(message.data().data(), message.data().size());

						g_my_id = info.my_id();
						INFO("myid {}", info.my_id());
						UnityBridge::Get().CallUnity<void>("SetMyId", info.my_id());

						InitGame(info.player_infos(),info.start_time(), info.my_id());
					}
					else if (message.name() == "ping")
					{
						Proto::Ping ping;
						ping.ParseFromArray(message.data().data(), message.data().size());

						uint32_t now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
						g_ping = now - ping.time();

						UnityBridge::Get().CallUnity<void>("SetPing", g_ping);
					}
					else
					{
						if (g_game_messages.find(message.name()) == g_game_messages.end())
						{
							g_game_messages.emplace(message.name(), std::queue<NetMessage>());
						}

						auto& messges = g_game_messages[message.name()];
						messges.emplace(std::move(message));
					}
				});

			g_network_service->Request("join_match", std::vector<uint8_t>{}, [](const std::vector<uint8_t>& buffer)
				{
					INFO("join_match!")
					// 偷懒不处理了
				});
		}
	}
}
 