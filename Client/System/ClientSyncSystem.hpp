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

	void Update(fixed16 dt) override
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

			INFO("frame {}", frame_data.frame);

			if (frame_data.frame >= game_state.run_frame)
			{
				MappingLocalEntity(frame_data, true);
				ReplicationServerData(frame_data,true);

				++game_state.run_frame;
				++game_state.verified_frame;

				auto& player_manager = registry.ctx<PlayerManager>();
				for (auto& item : player_manager.players)
				{
					item.second.local_id_generator = frame_data.local_id;
				}
			}
			else
			{
				bool is_need_rollback = CheckLocalEnity(frame_data);
				MappingLocalEntity(frame_data, is_need_rollback);

				is_need_rollback = (is_need_rollback ? is_need_rollback : CheckEntityState(frame_data));
				ReplicationServerData(frame_data, is_need_rollback);

				++game_state.verified_frame;

				if (is_need_rollback)
				{
					game_state.run_frame = frame_data.frame;

					auto& player_manager = registry.ctx<PlayerManager>();
					for (auto& item : player_manager.players)
					{
						item.second.local_id_generator = frame_data.local_id;
					}
				}
			}
		}
	}

	void Finalize() override
	{

	}

	bool CheckLocalEnity(FrameData& frame_data)
	{
		auto& local_id_state = registry.ctx<LocalIdState>();
		return frame_data.local_id != local_id_state.value[frame_data.frame % local_id_state.value.size()];
	}

	bool CheckEntityState(FrameData& frame_data)
	{
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
				
				INFO("frame {} local frame{} frame2 {} local x {} y{} z {} net", frame_data.frame, game_state.run_frame,game_state.verified_frame, tran.position.x, tran.position.y, tran.position.z, it->second.position.x, it->second.position.y, it->second.position.z);
				if (it->second != transform_state.value[frame_data.frame % transform_state.value.size()])
				{
					assert(false);
					return true;
				}
			}
			else
			{
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

			/*if (attribute_set_it->second.value != gameplay_state.attribute_sets[frame_data.frame % gameplay_state.attribute_sets.size()].value)
			{
				return true;
			}*/

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

	void MappingLocalEntity(FrameData& frame_data,bool is_need_rollback)
	{
		auto& e_by_local_id_map = registry.ctx<EntityByLocalIdMap>();
		for (auto& item : frame_data.unique_ids)
		{
			auto it = e_by_local_id_map.value.find(item.first);
			if (it != e_by_local_id_map.value.end())
			{
				if (!registry.has<UniqueId>(it->second))
				{
					INFO("e {} id{}", static_cast<uint32_t>(it->second),item.second.value);
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
			INFO("e {} id {} x {} y {} z {} ",static_cast<uint32_t>(e), unique_id.value, transform.position.x, transform.position.y, transform.position.z);
			if (is_need_rollback || !registry.has<TransformState>(e))
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
			if (!registry.has<LocalId>(e))
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
			if (!registry.has<LocalId>(e))
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
			if (is_need_rollback || !registry.has<TransformState>(e))
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
		}

		for (auto& item : frame_data.tags)
		{
			if (e_by_unique_id_map.value.find(item.first) == e_by_unique_id_map.value.end())
			{
				auto e = registry.create();
				registry.emplace<UniqueId>(e, item.first);
			}

			auto e = e_by_unique_id_map.value[item.first];
			if (is_need_rollback || !registry.has<GameplayState>(e))
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
			if (is_need_rollback || !registry.has<GameplayState>(owner))
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
			if (is_need_rollback || !registry.has<GameplayState>(owner))
			{
				registry.emplace_or_replace<GameplayEffectSpec>(e, item.second);
			}
		}
	}
};