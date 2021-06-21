#pragma once

#include "Framework/Game/System.hpp"

#include "Framework/Game/Proto.hpp"
#include "Framework/Game/Locator.hpp"
#include "Framework/Game/NetworkService.hpp"

#include "Framework/Game/Component/Transform.hpp"
#include "Framework/Game/Component/Player.hpp"
#include "Framework/Game/Component/TransformState.hpp"
#include "Framework/Game/Component/Local.hpp"

struct ClientSyncSystem : public System
{
	kanex::Buffer buffer;

	ClientSyncSystem(entt::registry& _registry)
		: System(_registry)
		, buffer(1024)
	{

	}

	~ClientSyncSystem()
	{

	}

	bool Initialize() override
	{

	}

	void Update(fixed16 dt) override
	{
		//auto view = registry_.view<Player, Transform>();
		//for (auto e : view)
		//{
		//	const auto& player = registry_.get<Player>(e);
		//	auto& transform = registry_.get<Transform>(e);

		//	auto iter = frame_data.find(player.id);
		//	if (iter != frame_data.end()) // 没变动角色不会下发
		//	{
		//		bool is_local = registry_.has<Local>(e);
		//		if (is_local && frame < run_frame_)
		//		{
		//			// todo 可以允许有一定误差,因为只是预测，最终服务器都会会纠正
		//			bool is_need_rollback = transform.position != iter->second.position || transform.rotation != iter->second.rotation;
		//			if (is_need_rollback)
		//			{
		//				transform = iter->second;
		//			}
		//		}
		//		else
		//		{
		//			registry_.replace<Transform>(e, iter->second);
		//		}
		//	}
		//}

		FrameData frame_data;
		bool is_need_rollback = false;
		auto view = registry.view<Player,TransformState,Local>();
		for (auto e : view)
		{
			auto&[player, transform_state,transform] = view.get<Player, TransformState,Transform>(e);
			auto& frame_transform = transform_state.transforms[frame_data.frame % transform_state.transforms.size()];
			
			auto iter = frame_data.actors.find(player.id);
			if (iter != frame_data.actors.end())
			{
				auto& sync_trans = iter->second.transform;
				is_need_rollback = frame_transform.position != sync_trans.position || frame_transform.rotation != sync_trans.rotation;
				if (is_need_rollback)
				{
					
				}
			}
		}

		int

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