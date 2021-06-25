#pragma once

#include "Framework/Game/System.hpp"

#include "Framework/Game/Proto.hpp"
#include "Framework/Game/Locator.hpp"
#include "Framework/Game/NetworkService.hpp"

#include "Framework/Game/Component/Transform.hpp"
#include "Framework/Game/Component/Player.hpp"
#include "Framework/Game/Component/TransformState.hpp"
#include "Framework/Game/Component/AnimationClip.hpp"
#include "Framework/Game/Component/Local.hpp"
#include "Framework/Game/Component/Remote.hpp"

#include "Kanex.hpp"

struct ClientSyncSystem : public System
{
	ClientSyncSystem(entt::registry& _registry)
		: System(_registry)
	{

	}

	~ClientSyncSystem()
	{

	}

	bool Initialize() override
	{
		return true;
	}

	void Update(fixed16 dt) override
	{
		auto& network_service = registry.ctx<Locator>().Ref<const NetworkService>();
		while (auto message = network_service.Recv())
		{
			auto&[name, data] = *message;

			kanex::Buffer buffer(std::move(data));
			kanex::BinaryStream stream(buffer);
			kanex::BinaryInputArchive ar(stream);

			FrameData frame_data;
			ar(frame_data);

			bool is_need_rollback = false;
			auto view = registry.view<Player,TransformState,Transform,Local>();
			for (auto e : view)
			{
				auto& [player, transform_state] = view.get<Player, TransformState>(e);
				auto& frame_transform = transform_state.value[frame_data.frame % transform_state.value.size()];

				auto iter = frame_data.actors.find(player.id);
				if (iter != frame_data.actors.end())
				{
					auto& sync_trans = iter->second.transform;

					//可以允许误差，因为最终服务器都会修正
					INFO("server {} client {}", sync_trans.rotation.w, frame_transform.rotation.w);

					if (frame_transform.position != sync_trans.position || frame_transform.rotation != sync_trans.rotation)
					{
						is_need_rollback = true;
						registry.replace<Transform>(e, sync_trans);
					}
				}
			}

			if (is_need_rollback)
			{
				INFO("-----------------------is_need_rollback");
				// todo : rollback
			}
			else
			{
				INFO("not rollback");
			}

			auto view_remote = registry.view<Player, Transform, AnimationClip, Remote>();
			for (auto e : view_remote)
			{
				auto& player = view_remote.get<Player>(e);
				auto iter = frame_data.actors.find(player.id);
				if (iter != frame_data.actors.end())
				{
					registry.replace<Transform>(e, iter->second.transform);
					registry.replace<AnimationClip>(e, iter->second.clip);
				}
			}
		}
	}

	void Finalize() override
	{

	}
};