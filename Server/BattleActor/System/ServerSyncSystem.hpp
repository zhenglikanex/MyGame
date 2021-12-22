#pragma once

#include "Framework/Game/System.hpp"

#include "Framework/Game/Proto.hpp"
#include "Framework/Game/Locator.hpp"
#include "Framework/Game/NetworkService.hpp"

struct ServerSyncSystem : public System
{
	kanex::Buffer buffer;

	ServerSyncSystem(entt::registry& _registry)
		: System(_registry)
		, buffer(1024)
	{
		
	}

	~ServerSyncSystem()
	{

	}

	bool Initialize() override
	{
		return true;
	}

	void FixedUpdate(fixed16 dt) override
	{
		FrameData frame_data;
		frame_data.frame = registry.ctx<GameState>().run_frame;
		
		auto view1 = registry.view<Transform>();
		for (auto e : view1)
		{
			auto& transform = view1.get<Transform>(e);
			frame_data.transforms.emplace(view1.get<UniqueId>(e), transform);
		}

		auto view2 = registry.view<AnimationClip>();
		for (auto e : view1)
		{
			auto& transform = view1.get<Transform>(e);
			frame_data.transforms.emplace(view1.get<UniqueId>(e), transform);
		}

		auto view3 = registry.view<>();
		for (auto e : view1)
		{
			auto& transform = view1.get<Transform>(e);
			frame_data.transforms.emplace(view1.get<UniqueId>(e), transform);
		}

		auto view1 = registry.view<Transform>();
		for (auto e : view1)
		{
			auto& transform = view1.get<Transform>(e);
			frame_data.transforms.emplace(view1.get<UniqueId>(e), transform);
		}

		auto view1 = registry.view<Transform>();
		for (auto e : view1)
		{
			auto& transform = view1.get<Transform>(e);
			frame_data.transforms.emplace(view1.get<UniqueId>(e), transform);
		}

		kanex::BinaryStream stream(buffer);
		kanex::BinaryOutputArchive oar(stream);
		oar(frame_data);

		std::vector<uint8_t> data(buffer.Size());
		buffer.Read(data.data(), data.size());

		buffer.Clear();

		auto& network_service = registry.ctx<Locator>().Ref<NetworkService>();
		network_service.Send("sync_frame_data", std::move(data));
	}

	void Finalize() override
	{

	}
};