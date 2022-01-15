#pragma once

#include "Framework/Game/System.hpp"

#include "Framework/Game/Proto.hpp"
#include "Framework/Game/Locator.hpp"
#include "Framework/Game/NetworkService.hpp"

#include "Framework/Game/Component/EntityByUniqueIdMap.hpp"
#include "Framework/Game/Component/EntityByLocalMap.hpp"
#include "Framework/Game/Component/SyncInstanceState.hpp"
#include "Framework/Game/Component/SyncManager.hpp"
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

	void FixedUpdate(fixed16 dt) override
	{
		auto& game_state = registry.ctx<GameState>();
		auto& network_service = registry.ctx<Locator>().Ref<const NetworkService>();
		INFO("sync");
		while (auto message = network_service.Recv())
		{
			auto& [name, data] = *message;

			kanex::Buffer buffer(std::move(data));
			kanex::BinaryStream stream(buffer);
			kanex::BinaryInputArchive ar(stream);

			FrameData frame_data;
			ar(frame_data);

			INFO("client frame {}", frame_data.frame);

			if (frame_data.frame >= game_state.run_frame)
			{
				MappingLocalEntity(frame_data);
				ReplicationServerLocalEntity(frame_data);
				ReplicationServerData(frame_data, true);

				game_state.run_frame = frame_data.frame;
				game_state.verified_frame = frame_data.frame;
			}
			else
			{
				MappingLocalEntity(frame_data);

				bool is_need_rollback = CheckLocalEnity(frame_data) || CheckEntityState(frame_data);
				if (is_need_rollback)
				{
					//这帧时间创建的所有local entity
					RollbackLocalEntity(frame_data);

					//恢复缺少local entity
					ReplicationServerLocalEntity(frame_data);
				}
				/*if (is_need_rollback)
				{
					MappingLocalEntity(frame_data, is_need_rollback);
				}*/

				ReplicationServerData(frame_data, is_need_rollback);

				game_state.verified_frame = frame_data.frame;

				if (is_need_rollback)
				{
					INFO("is need rollback  frame {}", frame_data.frame);
					game_state.run_frame = frame_data.frame;
				}
			}
		}

		INFO("END CLIENT SYNC");
	}

	void Finalize() override
	{

	}

	bool CheckLocalEnity(FrameData& frame_data)
	{
		auto& state = registry.ctx<SyncInstanceState>();
		auto result = frame_data.local_id != state.value[frame_data.frame % state.value.size()].identifier;
		if (result)
		{
			INFO("CheckLocalEnity rollback");
			return result;
		}

		return result;
	}

	bool CheckEntityState(FrameData& frame_data)
	{
		INFO("CheckEntityState {}", frame_data.frame);
		auto& game_state = registry.ctx<GameState>();
		auto view1 = registry.view<UniqueId, TransformState>();
		for (auto e : view1)
		{
			auto& unique_id = view1.get<UniqueId>(e);
			auto& transform_state = view1.get<TransformState>(e);

			auto it = frame_data.transforms.find(unique_id);
			if (it != frame_data.transforms.end())
			{
				auto tran = transform_state.value[frame_data.frame % transform_state.value.size()];

				if (it->second != transform_state.value[frame_data.frame % transform_state.value.size()])
				{
					INFO("position not!");
					return true;
				}
			}
			else
			{
				assert(false);
				return true;
			}
		}

		/*auto view = registry.view<UniqueId, ActorState>();
		for (auto e : view)
		{
			auto& unique_id = view1.get<UniqueId>(e);
			auto& transform_state = view1.get<ActorState>(e);

			auto it = frame_data.transforms.find(unique_id);
			if (it != frame_data.transforms.end())
			{
				auto tran = transform_state.value[frame_data.frame % transform_state.value.size()];

				if (it->second != transform_state.value[frame_data.frame % transform_state.value.size()])
				{
					INFO("position not!");
					return true;
				}
			}
			else
			{
				assert(false);
				return true;
			}
		}*/

		auto view2 = registry.view<UniqueId, GameplayState>();
		for (auto e : view2)
		{
			auto& unique_id = view2.get<UniqueId>(e);
			auto& gameplay_state = view2.get<GameplayState>(e);

			auto tag_it = frame_data.tags.find(unique_id);
			auto attribute_set_it = frame_data.attribute_sets.find(unique_id);

			if (attribute_set_it == frame_data.attribute_sets.end())
			{
				return true;
			}

			if (attribute_set_it->second.value != gameplay_state.attribute_sets[frame_data.frame % gameplay_state.attribute_sets.size()].value)
			{
				return true;
			}

			if (tag_it == frame_data.tags.end() && !gameplay_state.tags[frame_data.frame % gameplay_state.tags.size()].tags.Empty())
			{
				return true;
			}

			if (tag_it != frame_data.tags.end() && tag_it->second.tags != gameplay_state.tags[frame_data.frame % gameplay_state.tags.size()].tags)
			{
				return true;
			}
		}

		return false;
	}

	void RollbackLocalEntity(FrameData& frame_data)
	{
		if (frame_data.frame <= 0)
		{
			return;
		}

		auto& e_by_local_id_map = registry.ctx<EntityByLocalMap>();
		auto& sync_manager = registry.ctx<SyncManager>();

		// 删除多创建的local entity
		for (auto& item : sync_manager.sync_instances)
		{
			for (auto i = frame_data.local_id; i < item.second.identifier; ++i)
			{
				auto it = e_by_local_id_map.value.find(Local(item.second.owner, i));
				if (it != e_by_local_id_map.value.end())
				{
					registry.destroy(it->second);
				}
			}
		}

	}

	void MappingLocalEntity(FrameData& frame_data)
	{
		auto& e_by_local_id_map = registry.ctx<EntityByLocalMap>();
		for (auto& item : frame_data.unique_ids)
		{
			auto it = e_by_local_id_map.value.find(item.first);
			if (it != e_by_local_id_map.value.end())
			{
				if (!registry.all_of<UniqueId>(it->second))
				{
					registry.emplace<UniqueId>(it->second, item.second);
				}
			}
		}
	}

	void ReplicationServerLocalEntity(FrameData& frame_data)
	{
		auto& e_by_local_id_map = registry.ctx<EntityByLocalMap>();
		for (auto& item : frame_data.unique_ids)
		{
			auto it = e_by_local_id_map.value.find(item.first);
			if (it == e_by_local_id_map.value.end())
			{
				auto e = registry.create();
				registry.emplace<Local>(e, item.first);
				registry.emplace<UniqueId>(e, item.second);
			}
		}
	}

	void ReplicationServerData(FrameData& frame_data, bool is_need_rollback)
	{
		auto& unique_map = registry.ctx<EntityByUniqueIdMap>();

		for (auto& item : frame_data.transforms)
		{
			if (unique_map.value.find(item.first) == unique_map.value.end())
			{
				auto e = registry.create();
				registry.emplace<UniqueId>(e, item.first);
			}

			auto e = unique_map.value[item.first];
			auto& transform = item.second;
			auto& unique_id = item.first;
			if (is_need_rollback || !registry.all_of<TransformState>(e))
			{
				registry.emplace_or_replace<Transform>(e, item.second);
			}
		}

		for (auto& item : frame_data.players)
		{
			if (unique_map.value.find(item.first) == unique_map.value.end())
			{
				auto e = registry.create();
				registry.emplace<UniqueId>(e, item.first);
			}

			auto e = unique_map.value[item.first];
			if (!registry.all_of<Local>(e))
			{
				registry.emplace_or_replace<Player>(e, item.second);
			}
		}

		for (auto& item : frame_data.actor_assets)
		{
			if (unique_map.value.find(item.first) == unique_map.value.end())
			{
				auto e = registry.create();
				registry.emplace<UniqueId>(e, item.first);
			}

			auto e = unique_map.value[item.first];
			if (!registry.all_of<Local>(e))
			{
				registry.emplace_or_replace<ActorAsset>(e, item.second);
			}
		}

		for (auto& item : frame_data.view_assets)
		{
			if (unique_map.value.find(item.first) == unique_map.value.end())
			{
				auto e = registry.create();
				registry.emplace<UniqueId>(e, item.first);
			}

			auto e = unique_map.value[item.first];
			if (!registry.all_of<Local>(e))
			{
				registry.emplace_or_replace<ViewAsset>(e, item.second);
			}
		}

		for (auto& item : frame_data.animation_clips)
		{
			if (unique_map.value.find(item.first) == unique_map.value.end())
			{
				auto e = registry.create();
				registry.emplace<UniqueId>(e, item.first);
			}

			auto e = unique_map.value[item.first];
			if (is_need_rollback || !registry.all_of<TransformState>(e))
			{
				registry.emplace_or_replace<AnimationClip>(e, item.second);
			}
		}

		for (auto& item : frame_data.attribute_sets)
		{
			if (unique_map.value.find(item.first) == unique_map.value.end())
			{
				auto e = registry.create();
				registry.emplace<UniqueId>(e, item.first);
			}

			// 这个不预测
			auto e = unique_map.value[item.first];
			registry.emplace_or_replace<GameplayAttributeSet>(e, item.second);
		}

		for (auto& item : frame_data.tags)
		{
			if (unique_map.value.find(item.first) == unique_map.value.end())
			{
				auto e = registry.create();
				registry.emplace<UniqueId>(e, item.first);
			}

			auto e = unique_map.value[item.first];
			if (is_need_rollback || !registry.all_of<GameplayState>(e))
			{
				registry.emplace_or_replace<GameplayTags>(e, item.second);
			}
		}

		for (auto& item : frame_data.ability_specs)
		{
			if (unique_map.value.find(item.first) == unique_map.value.end())
			{
				auto e = registry.create();
				registry.emplace<UniqueId>(e, item.first);
			}

			auto e = unique_map.value[item.first];
			auto owner = unique_map.value[item.second.source];
			if (is_need_rollback || !registry.all_of<GameplayState>(owner))
			{
				registry.emplace_or_replace<GameplayAbilitySpec>(e, item.second);
			}
		}

		for (auto& item : frame_data.effect_specs)
		{
			if (unique_map.value.find(item.first) == unique_map.value.end())
			{
				auto e = registry.create();
				registry.emplace<UniqueId>(e, item.first);
			}

			auto e = unique_map.value[item.first];
			auto owner = unique_map.value[item.second.owner];
			if (is_need_rollback || !registry.all_of<GameplayState>(owner))
			{
				registry.emplace_or_replace<GameplayEffectSpec>(e, item.second);
			}
		}

		for (auto& item : frame_data.destorys)
		{
			auto it = unique_map.value.find(item.value);
			if (it != unique_map.value.end())
			{
				entt::entity e = it->second;
				if (!registry.all_of<Destory>(e))
				{
					registry.emplace<Destory>(e);
				}
			}
		}

		if (is_need_rollback)
		{
			auto& local_map = registry.ctx<EntityByLocalMap>();
			for (auto& item : frame_data.wait_tasks)
			{
				auto it = local_map.value.find(item.first);

				assert(it != local_map.value.end() && "这里不可能不对应");

				registry.emplace_or_replace<GameplayWaitTask>(it->second, item.second);
			}

			for (auto& item : frame_data.movement_forwards)
			{
				auto it = local_map.value.find(item.first);

				assert(it != local_map.value.end() && "这里不可能不对应");

				registry.emplace_or_replace<MovementForward>(it->second, item.second);
			}

			for (auto& item : frame_data.play_anim_and_wait_tasks)
			{
				auto it = local_map.value.find(item.first);

				assert(it != local_map.value.end() && "这里不可能不对应");

				registry.emplace_or_replace<GameplayPlayAnimAndWaitTask>(it->second, item.second);
			}

			for (auto& item : frame_data.input_tasks)
			{
				auto it = local_map.value.find(item.first);

				assert(it != local_map.value.end() && "这里不可能不对应");

				registry.emplace_or_replace<GameplayInputTask>(it->second, item.second);
			}

			for (auto& item : frame_data.hit_test_tasks)
			{
				auto it = local_map.value.find(item.first);

				assert(it != local_map.value.end() && "这里不可能不对应");
			
				registry.emplace_or_replace<GameplayHitTestTask>(it->second, item.second);
			}

			for (auto& item : frame_data.character_movements)
			{
				auto it = local_map.value.find(item.first);

				assert(it != local_map.value.end() && "这里不可能不对应");

				auto& comp = registry.get_or_emplace<CharacterMovement>(it->second);
				comp.base_velocity = item.second.base_velocity;
				comp.attached_rigidbody_velocity = item.second.attached_rigidbody_velocity;
				comp.grounding_status.ground_point = item.second.grounding_status.ground_point;
				comp.last_grounding_status.ground_point = item.second.last_grounding_status.ground_point;
			}

			auto trans_state_view = registry.view<TransformState, Transform>();
			for (auto e : trans_state_view)
			{
				auto& [transform_state, transform] = trans_state_view.get<TransformState, Transform>(e);
				transform_state.value[frame_data.frame % transform_state.value.size()] = transform;
			}

			auto view = registry.view<GameplayState, GameplayAttributeSet>();
			for (auto e : view)
			{
				auto& [gameplay_state, attribute_set] = view.get<GameplayState, GameplayAttributeSet>(e);
				gameplay_state.attribute_sets[frame_data.frame % gameplay_state.attribute_sets.size()] = attribute_set;

				auto tags = registry.try_get<GameplayTags>(e);
				if (tags)
				{
					gameplay_state.tags[frame_data.frame % gameplay_state.tags.size()] = *tags;
				}
			}

			auto& state = registry.ctx<SyncInstanceState>();
			auto& sync_manager = registry.ctx<SyncManager>();
			for (auto& item : sync_manager.sync_instances)
			{
				item.second.identifier = frame_data.local_id;
				state.value[frame_data.frame % state.value.size()].identifier = item.second.identifier;
			}
		}
	}
};