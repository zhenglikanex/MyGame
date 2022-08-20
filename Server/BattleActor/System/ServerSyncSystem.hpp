#pragma once

#include <any>

#include "Framework/Game/System.hpp"

#include "Framework/Game/Locator.hpp"
#include "Framework/Game/SerializationManager.hpp"
#include "Framework/Game/Service/NetworkService.hpp"

#include "Framework/Game/Component/SyncManager.hpp"

#include "Server/BattleActor/ServerNetworkService.hpp"

using namespace std;
using namespace std::chrono;

struct ServerSyncSystem : public System
{
	SerializationManager serialization_manager;

	ServerSyncSystem(Registry& _registry)
		: System(_registry)
	{

	}

	bool Initialize() override
	{
		serialization_manager.InitComponentIds();
		return true;
	}

	void FixedUpdate(float dt) override
	{
		serialization_manager.Clear();

		auto& sync_manager = registry.ctx<SyncManager>();
		for (auto& item : sync_manager.sync_instances)
		{
			const SyncInstance& sync_instance = item.second;

			if (sync_instance.owner == ServerInstance)
			{
				continue;
			}

			serialization_manager.Serialize(registry,sync_instance);

			std::vector<uint8_t> buffer( serialization_manager.buffer().Size());
			serialization_manager.buffer().Read(buffer.data(), buffer.size());
			auto& network_service = dynamic_cast<ServerNetworkService&>(registry.ctx<Locator>().Ref<NetworkService>());
			network_service.Send(sync_instance.owner, "sync_frame_data", std::move(buffer));
		}
	}

	void Finalize() override
	{

	}
};