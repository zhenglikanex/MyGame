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
		return true;
	}

	void FixedUpdate(float dt) override
	{
		serialization_manager.Clear();
		serialization_manager.SerializeCommon(registry);

		std::vector<uint8_t> common_buffer(serialization_manager.buffer().Size());
		serialization_manager.buffer().Read(common_buffer.data(), common_buffer.size());

		auto& sync_manager = registry.ctx<SyncManager>();
		for (auto& item : sync_manager.sync_instances)
		{
			const SyncInstance& sync_instance = item.second;

			if (sync_instance.owner == ServerInstance)
			{
				continue;
			}

			serialization_manager.Clear();
			serialization_manager.SerializeInstance(registry,sync_instance);

			std::vector<uint8_t> buffer(common_buffer.size() + serialization_manager.buffer().Size());
			std::copy_n(common_buffer.begin(), common_buffer.size(), buffer.data());
			serialization_manager.buffer().Read(buffer.data() + common_buffer.size(), serialization_manager.buffer().Size());

			auto& network_service = dynamic_cast<ServerNetworkService&>(registry.ctx<Locator>().Ref<NetworkService>());
			network_service.Send(sync_instance.owner, "sync_frame_data", std::move(buffer));
		}
	}

	void Finalize() override
	{

	}
};