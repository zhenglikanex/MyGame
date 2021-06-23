#pragma once

#include "Framework/Game/System.hpp"

#include "Framework/Game/Proto.hpp"
#include "Framework/Game/Locator.hpp"
#include "Framework/Game/NetworkService.hpp"

#include "Framework/Game/Component/Transform.hpp"
#include "Framework/Game/Component/Player.hpp"
#include "Framework/Game/Component/TransformState.hpp"
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
		auto& network_service = registry.ctx<Locator>().Ref<const NetworkService&>();
		while (auto message = network_service.Recv())
		{
			auto&[name, data] = *message;

			kanex::Buffer buffer(std::move(data));
			kanex::BinaryStream stream(buffer);
			kanex::BinaryInputArchive ar(stream);

			FrameData frame_data;
			ar(frame_data);

			bool is_need_rollback = false;
			auto view = registry.view<Player, TransformState, Transform, Local>();
			for (auto e : view)
			{
				auto&[player, transform_state, transform] = view.get<Player, TransformState, Transform>(e);
				auto& frame_transform = transform_state.transforms[frame_data.frame % transform_state.transforms.size()];

				auto iter = frame_data.actors.find(player.id);
				if (iter != frame_data.actors.end())
				{
					auto& sync_trans = iter->second.transform;
					//可以允许误差，因为最终服务器都会修正
					is_need_rollback = frame_transform.position != sync_trans.position || frame_transform.rotation != sync_trans.rotation;
					if (is_need_rollback)
					{
						transform = sync_trans;
					}
				}
			}

			if (is_need_rollback)
			{
				// todo : rollback
			}

			auto view_remote = registry.view<Player, Transform, Remote>();
			for (auto e : view)
			{
				auto&[player, transform] = view_remote.get<Player, Transform>(e);

				auto iter = frame_data.actors.find(player.id);
				if (iter != frame_data.actors.end())
				{
					transform = iter->second.transform;
				}
			}
		}
	}

	void Finalize() override
	{

	}
};