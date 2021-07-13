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
#include "Framework/Game/Component/GameState.hpp"

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
		auto& game_state = registry.ctx<GameState>();
		auto& network_service = registry.ctx<Locator>().Ref<const NetworkService>();
		while (auto message = network_service.Recv())
		{
			auto&[name, data] = *message;

			kanex::Buffer buffer(std::move(data));
			kanex::BinaryStream stream(buffer);
			kanex::BinaryInputArchive ar(stream);

			FrameData frame_data;
			ar(frame_data);

			//
			if (frame_data.frame >= game_state.run_frame)
			{
				ApplyServerData(frame_data);
				++game_state.run_frame;
				++game_state.verified_frame;

				INFO("game_state.run_frame {} ", game_state.run_frame);
				INFO("game_state.verified_frame {} ", game_state.verified_frame);
			}
			else
			{
				VerifyClientPrediction(frame_data);
				++game_state.verified_frame;
				INFO("!!!!!!!!!!!!!!!!!!!!!!!!!!!!game_state.run_frame {} ", game_state.run_frame);
			}
		}
	}

	void Finalize() override
	{

	}

	void ApplyServerData(FrameData& frame_data)
	{
		auto view = registry.view<Player,Transform,AnimationClip>();
		for (auto e : view)
		{
			auto& player = view.get<Player>(e);
			auto iter = frame_data.actors.find(player.id);
			if (iter != frame_data.actors.end())
			{
				registry.replace<Transform>(e, iter->second.transform);
				registry.replace<AnimationClip>(e, iter->second.clip);
			}
		}

		auto view2 = registry.view<Player, TransformState,Transform>();
		for (auto e : view2)
		{
			auto& [player,transform_state,transform] = view2.get<Player, TransformState,Transform>(e);
			transform_state.value[frame_data.frame % transform_state.value.size()] = transform;
		}
	}

	void VerifyClientPrediction(FrameData& frame_data)
	{
		bool is_need_rollback = false;
		auto view = registry.view<Player, TransformState, Transform, Local>();
		for (auto e : view)
		{
			auto& [player, transform_state] = view.get<Player, TransformState>(e);
			auto& frame_transform = transform_state.value[frame_data.frame % transform_state.value.size()];

			auto iter = frame_data.actors.find(player.id);
			if (iter != frame_data.actors.end())
			{
				auto& sync_trans = iter->second.transform;

				//可以允许误差，因为最终服务器都会修正
				//INFO("server {} {} {} {}  client {} {} {} {}", sync_trans.position.x, sync_trans.position.y, sync_trans.position.z, sync_trans.position.w, sync_trans.position.x, sync_trans.position.y, sync_trans.position.z, transform.position.w);
				if (frame_transform.position != sync_trans.position || frame_transform.rotation != sync_trans.rotation)
				{
					is_need_rollback = true;
					registry.replace<Transform>(e, sync_trans);
				}
			}
		}

		if (is_need_rollback)
		{
			registry.ctx<GameState>().run_frame = frame_data.frame + 1;
			// todo : rollback
		}
		else
		{
			//INFO("not rollback");
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
};