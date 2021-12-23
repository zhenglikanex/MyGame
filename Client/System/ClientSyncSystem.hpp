#pragma once

#include "Framework/Game/System.hpp"

#include "Framework/Game/Proto.hpp"
#include "Framework/Game/Locator.hpp"
#include "Framework/Game/NetworkService.hpp"

#include "Framework/Game/Component/EntityByUniqueIdMap.hpp"
#include "Framework/Game/Component/"
#include "Framework/Game/Component/Transform.hpp"
#include "Framework/Game/Component/Player.hpp"
#include "Framework/Game/Component/TransformState.hpp"
#include "Framework/Game/Component/AnimationClip.hpp"
#include "Framework/Game/Component/GameState.hpp"
#include "Framework/Game/Component/GAS/GameplayState.hpp"

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
				//1.先验证是否需要回滚

				//验证失败需要回滚
				//1.根据localid给本地entity添加上uniqueid
				//2.删除没有uniqueid的本地entity
				//3.覆盖预测的数据
				//验证成功不需要回滚
				//1.根据localid给本地entity添加上uniqueid

				if (IsNeedRollback(frame_data))
				{
					
				}
				else
				{

				}

				VerifyClientPrediction(frame_data);
				++game_state.verified_frame;
				INFO("!!!!!!!!!!!!!!!!!!!!!!!!!!!!game_state.run_frame {} ", game_state.run_frame);
			}
		}
	}

	void Finalize() override
	{

	}

	bool IsNeedRollback(FrameData& frame_data)
	{
		auto view1 = registry.view<TransformState>();
		for (auto e : view1)
		{
			auto& transform_state = view1.get<TransformState>(e);
			if (true)
			{
				return true;
			}
		}

		auto view2 = registry.view<GameplayState>();
		for (auto e : view2)
		{
			auto& gameplay_state = view2.get<GameplayState>(e);
			if (true)
			{
				return true;
			}
		}

		return false;
	}

	void ApplyServerData(FrameData& frame_data)
	{
		auto& e_by_unique_id_map = registry.ctx<EntityByUniqueIdMap>();

		auto view = registry.view<LocalId>(entt::exclude<UniqueId>);
		for (auto& e : view)
		{
			auto& local_id = view.get<LocalId>(e);
			auto it = frame_data.unique_ids.find(local_id);
			if (it != frame_data.unique_ids.end())
			{
				registry.emplace<UniqueId>(e, it->second);
			}
			else 
			{
				// todo:应该删除这个
			}
		}


		for (auto& item : frame_data.transforms)
		{
			if (e_by_unique_id_map.value.find(item.first) != e_by_unique_id_map.value.end())
			{
				auto e = registry.create();
				registry.emplace<UniqueId>(e, item.first);
			}
			auto e = e_by_unique_id_map.value[item.first];
			registry.emplace_or_replace<Transform>(e, item.second);
		}

		for (auto& item : frame_data.animation_clips)
		{
			if (e_by_unique_id_map.value.find(item.first) != e_by_unique_id_map.value.end())
			{
				auto e = registry.create();
				registry.emplace<UniqueId>(e, item.first);
			}
			auto e = e_by_unique_id_map.value[item.first];
			registry.emplace_or_replace<AnimationClip>(e, item.second);
		}

		for (auto& item : frame_data.attribute_sets)
		{
			if (e_by_unique_id_map.value.find(item.first) != e_by_unique_id_map.value.end())
			{
				auto e = registry.create();
				registry.emplace<UniqueId>(e, item.first);
			}
			auto e = e_by_unique_id_map.value[item.first];
			registry.emplace_or_replace<GameplayAttributeSet>(e, item.second);
		}

		for (auto& item : frame_data.tags)
		{
			if (e_by_unique_id_map.value.find(item.first) != e_by_unique_id_map.value.end())
			{
				auto e = registry.create();
				registry.emplace<UniqueId>(e, item.first);
			}
			auto e = e_by_unique_id_map.value[item.first];
			registry.emplace_or_replace<GameplayTags>(e, item.second);
		}

		for (auto& item : frame_data.ability_specs)
		{
			if (e_by_unique_id_map.value.find(item.first) != e_by_unique_id_map.value.end())
			{
				auto e = registry.create();
				registry.emplace<UniqueId>(e, item.first);
			}
			auto e = e_by_unique_id_map.value[item.first];
			registry.emplace_or_replace<GameplayAbilitySpec>(e, item.second);
		}

		for (auto& item : frame_data.effect_specs)
		{
			if (e_by_unique_id_map.value.find(item.first) != e_by_unique_id_map.value.end())
			{
				auto e = registry.create();
				registry.emplace<UniqueId>(e, item.first);
			}
			auto e = e_by_unique_id_map.value[item.first];
			registry.emplace_or_replace<GameplayEffectSpec>(e, item.second);
		}
	}

	void VerifyClientPrediction(FrameData& frame_data)
	{
		auto view = registry.view<TransformState>();
		for (auto e : view)
		{
			
		}

		//bool is_need_rollback = false;
		//auto view = registry.view<Player, TransformState, Transform, Local>();
		//for (auto e : view)
		//{
		//	auto& [player, transform_state] = view.get<Player, TransformState>(e);
		//	auto& frame_transform = transform_state.value[frame_data.frame % transform_state.value.size()];

		//	auto iter = frame_data.actors.find(player.id);
		//	if (iter != frame_data.actors.end())
		//	{
		//		auto& sync_trans = iter->second.transform;

		//		//可以允许误差，因为最终服务器都会修正
		//		//INFO("server {} {} {} {}  client {} {} {} {}", sync_trans.position.x, sync_trans.position.y, sync_trans.position.z, sync_trans.position.w, sync_trans.position.x, sync_trans.position.y, sync_trans.position.z, transform.position.w);
		//		if (frame_transform.position != sync_trans.position || frame_transform.rotation != sync_trans.rotation)
		//		{
		//			is_need_rollback = true;
		//			registry.replace<Transform>(e, sync_trans);
		//			INFO("is_need_rollback");
		//		}
		//	}
		//}

		//if (is_need_rollback)
		//{
		//	registry.ctx<GameState>().run_frame = frame_data.frame + 1;
		//	// todo : rollback
		//}
		//else
		//{
		//	//INFO("not rollback");
		//}

		//auto view_remote = registry.view<Player, Transform, AnimationClip, Remote>();
		//for (auto e : view_remote)
		//{
		//	auto& player = view_remote.get<Player>(e);
		//	auto iter = frame_data.actors.find(player.id);
		//	if (iter != frame_data.actors.end())
		//	{
		//		registry.replace<Transform>(e, iter->second.transform);
		//		registry.replace<AnimationClip>(e, iter->second.clip);
		//	}
		//}
	}

	void VerifyGameplayPrediction(FrameData& frame_data)
	{
		//检查属性是否一致
		//检查tag是否一致
		//ga,ge这两个可以通过上面两个判断出来了
		/*auto view = registry.view<Player,GameplayState,Local>();
		for (auto e : view)
		{
			auto& [player,gameplay_state] = view.get<Player,GameplayState>(e);
			auto iter = frame_data.actors.find(player.id);
			if (iter != frame_data.actors.end())
			{
				auto& frame_attribute_set = gameplay_state.attribute_sets[frame_data.frame % gameplay_state.attribute_sets.size()];
				auto& frame_tags = gameplay_state.tags[frame_data.frame % gameplay_state.tags.size()];
			}
		}*/
	}
};