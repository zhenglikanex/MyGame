#pragma once

#include "Framework/Game/System.hpp"

#include "Framework/Game/Proto.hpp"
#include "Framework/Game/Locator.hpp"
#include "Framework/Game/NetworkService.hpp"

#include "Framework/Game/Component/GameState.hpp"
#include "Framework/Game/Component/Transform.hpp"
#include "Framework/Game/Component/AnimationClip.hpp"
#include "Framework/Game/Component/Player.hpp"

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

		auto view = registry.view<Player, Transform, AnimationClip>();
		for (auto e : view)
		{
			auto&[player, transform,clip] = view.get<Player, Transform,AnimationClip>(e);
			
			ActorData actor_data;
			actor_data.transform = transform;
			actor_data.clip = clip;
			INFO("CLIP {}", clip.name);
			frame_data.actors.emplace(player.id, std::move(actor_data));
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