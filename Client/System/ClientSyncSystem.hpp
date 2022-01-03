#pragma once

#include "Framework/Game/System.hpp"

#include "Framework/Game/Proto.hpp"
#include "Framework/Game/Locator.hpp"
#include "Framework/Game/NetworkService.hpp"

#include "Framework/Game/Component/EntityByUniqueIdMap.hpp"
#include "Framework/Game/Component/EntityByLocalIdMap.hpp"
#include "Framework/Game/Component/LocalIdState.hpp"
#include "Framework/Game/Component/PlayerManager.hpp"
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
			auto&[name, data] = *message;

			kanex::Buffer buffer(std::move(data));
			kanex::BinaryStream stream(buffer);
			kanex::BinaryInputArchive ar(stream);

			FrameData frame_data;
			ar(frame_data);

			INFO("client frame {}", frame_data.frame);

			if (frame_data.frame >= game_state.run_frame)
			{
				MappingLocalEntity(frame_data, true);
				ReplicationServerData(frame_data,true);

				game_state.run_frame = frame_data.frame;
				game_state.verified_frame = frame_data.frame;
			}
			else
			{
				bool is_need_rollback = CheckLocalEnity(frame_data);

				if (is_need_rollback)
				{
					//这帧时间创建的所有local entity
					RollbackLocalEntity(frame_data);
				}

				MappingLocalEntity(frame_data, is_need_rollback);

				is_need_rollback = (is_need_rollback ? is_need_rollback : CheckEntityState(frame_data));
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
		auto& local_id_state = registry.ctx<LocalIdState>();
		auto result = frame_data.local_id != local_id_state.value[frame_data.frame % local_id_state.value.size()];
		if (result)
		{
			INFO("CheckLocalEnity rollback");
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
		assert(frame_data.frame > 0 && "第一帧不可能出错!");

		auto& e_by_local_id_map = registry.ctx<EntityByLocalIdMap>();

		auto& local_id_state = registry.ctx<LocalIdState>();
		for (auto i = local_id_state.value[(frame_data.frame - 1) % local_id_state.value.size()].value;
			i < frame_data.local_id.value; ++i)
		{
			auto it = e_by_local_id_map.value.find(i);
			if (it != e_by_local_id_map.value.end())
			{
				registry.destroy(it->second);
			}
		}
	}

	void MappingLocalEntity(FrameData& frame_data,bool is_need_rollback)
	{
		auto& e_by_local_id_map = registry.ctx<EntityByLocalIdMap>();
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
			else
			{
				// 只有验证
				assert(is_need_rollback && "只有验证失败才可以进来!");

				auto e = registry.create();
				registry.emplace<LocalId>(e, item.first);
				registry.emplace<UniqueId>(e, item.second);
			}
		}
	}
	
	void ReplicationServerData(FrameData& frame_data,bool is_need_rollback)
	{
		auto& e_by_unique_id_map = registry.ctx<EntityByUniqueIdMap>();

		for (auto& item : frame_data.transforms)
		{
			if (e_by_unique_id_map.value.find(item.first) == e_by_unique_id_map.value.end())
			{
				auto e = registry.create();
				registry.emplace<UniqueId>(e, item.first);
			}

			auto e = e_by_unique_id_map.value[item.first];
			auto& transform = item.second;
			auto& unique_id = item.first;
			if (is_need_rollback || !registry.all_of<TransformState>(e))
			{
				registry.emplace_or_replace<Transform>(e, item.second);
			}
		}

		for (auto& item : frame_data.players)
		{
			if (e_by_unique_id_map.value.find(item.first) == e_by_unique_id_map.value.end())
			{
				auto e = registry.create();
				registry.emplace<UniqueId>(e, item.first);
			}

			auto e = e_by_unique_id_map.value[item.first];
			if (!registry.all_of<LocalId>(e))
			{
				registry.emplace_or_replace<Player>(e, item.second);
			}
		}

		for (auto& item : frame_data.actor_assets)
		{
			if (e_by_unique_id_map.value.find(item.first) == e_by_unique_id_map.value.end())
			{
				auto e = registry.create();
				registry.emplace<UniqueId>(e, item.first);
			}

			auto e = e_by_unique_id_map.value[item.first];
			if (!registry.all_of<LocalId>(e))
			{
				registry.emplace_or_replace<ActorAsset>(e, item.second);
			}
		}

		for (auto& item : frame_data.animation_clips)
		{
			if (e_by_unique_id_map.value.find(item.first) == e_by_unique_id_map.value.end())
			{
				auto e = registry.create();
				registry.emplace<UniqueId>(e, item.first);
			}

			auto e = e_by_unique_id_map.value[item.first];
			if (is_need_rollback || !registry.all_of<TransformState>(e))
			{
				registry.emplace_or_replace<AnimationClip>(e, item.second);
			}
		}

		for (auto& item : frame_data.attribute_sets)
		{
			if (e_by_unique_id_map.value.find(item.first) == e_by_unique_id_map.value.end())
			{
				auto e = registry.create();
				registry.emplace<UniqueId>(e, item.first);
			}

			// 这个不预测
			auto e = e_by_unique_id_map.value[item.first];
			registry.emplace_or_replace<GameplayAttributeSet>(e, item.second);
			if (registry.all_of<LocalId>(e))
			{
				int a = 10;
			}
		}

		for (auto& item : frame_data.tags)
		{
			if (e_by_unique_id_map.value.find(item.first) == e_by_unique_id_map.value.end())
			{
				auto e = registry.create();
				registry.emplace<UniqueId>(e, item.first);
			}

			auto e = e_by_unique_id_map.value[item.first];
			if (is_need_rollback || !registry.all_of<GameplayState>(e))
			{
				registry.emplace_or_replace<GameplayTags>(e, item.second);
			}
		}

		for (auto& item : frame_data.ability_specs)
		{
			if (e_by_unique_id_map.value.find(item.first) == e_by_unique_id_map.value.end())
			{
				auto e = registry.create();
				registry.emplace<UniqueId>(e, item.first);
			}

			auto e = e_by_unique_id_map.value[item.first];
			auto owner = e_by_unique_id_map.value[item.second.source];
			if (is_need_rollback || !registry.all_of<GameplayState>(owner))
			{
				registry.emplace_or_replace<GameplayAbilitySpec>(e, item.second);
			}
		}

		for (auto& item : frame_data.effect_specs)
		{
			if (e_by_unique_id_map.value.find(item.first) == e_by_unique_id_map.value.end())
			{
				auto e = registry.create();
				registry.emplace<UniqueId>(e, item.first);
			}

			auto e = e_by_unique_id_map.value[item.first];
			auto owner = e_by_unique_id_map.value[item.second.owner];
			if (is_need_rollback || !registry.all_of<GameplayState>(owner))
			{
				registry.emplace_or_replace<GameplayEffectSpec>(e, item.second);
			}
		}

		if (is_need_rollback)
		{
			auto& e_by_local_id_map = registry.ctx<EntityByLocalIdMap>();
			for (auto& item : frame_data.wait_tasks)
			{
				auto it = e_by_local_id_map.value.find(item.first);

				assert(it != e_by_local_id_map.value.end() && "这里不可能不对应");

				registry.emplace_or_replace<GameplayWaitTask>(it->second, item.second);
			}

			for (auto& item : frame_data.play_anim_and_wait_tasks)
			{
				auto it = e_by_local_id_map.value.find(item.first);

				assert(it != e_by_local_id_map.value.end() && "这里不可能不对应");

				registry.emplace_or_replace<GameplayPlayAnimAndWaitTask>(it->second, item.second);
			}

			auto chara_movement_view = registry.view<UniqueId, CharacterMovement>();
			for (auto e : chara_movement_view)
			{
				auto& [id, chara_movement] = chara_movement_view.get<UniqueId, CharacterMovement>(e);
				
				auto it = frame_data.character_movements.find(id);
				if (it != frame_data.character_movements.end())
				{
					chara_movement.base_velocity = it->second.base_velocity;
					chara_movement.attached_rigidbody_velocity = it->second.attached_rigidbody_velocity;
					chara_movement.grounding_status.ground_point = it->second.grounding_status.ground_point;
					chara_movement.last_grounding_status.ground_point = it->second.last_grounding_status.ground_point;
				}
				else 
				{
					assert(false);
				}
			}

			auto trans_state_view = registry.view<TransformState, Transform>();
			for (auto e : trans_state_view)
			{
				auto& [transform_state,transform] = trans_state_view.get<TransformState, Transform>(e);
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

			auto& state = registry.ctx<LocalIdState>();
			auto& player_manager = registry.ctx<PlayerManager>();
			for (auto& item : player_manager.players)
			{
				item.second.local_id_generator = frame_data.local_id;
				state.value[frame_data.frame % state.value.size()] = item.second.local_id_generator;
			}
		}
	}
};