#pragma once

#include "Framework/Game/System.hpp"

#include "Framework/Game/SerializationManager.hpp"
#include "Framework/Game/Locator.hpp"
#include "Framework/Game/NetworkService.hpp"

#include "Framework/Game/Component/SyncManager.hpp"
#include "Framework/Game/Component/LocalEntityState.hpp"
#include "Framework/Game/Component/TransformState.hpp"
#include "Framework/Game/Component/GameState.hpp"
#include "Framework/Game/Component/GAS/GameplayState.hpp"

#include "Kanex.hpp"

#include "3rdparty/include/glm/gtx/vector_angle.hpp"

struct ClientSyncSystem : public System
{
	SerializationManager serialization_manager;

	ClientSyncSystem(Registry& _registry)
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

	void FixedUpdate(float dt) override
	{
		auto& network_service = registry.ctx<Locator>().Ref<const NetworkService>();
		while (auto message = network_service.Recv())
		{
			auto&[name, data] = *message;

			serialization_manager.Deserialize(std::move(data));

			auto& frame_data = serialization_manager.frame_data();
			auto& game_state = registry.ctx<GameState>();

			game_state.verified_frame = frame_data.frame;

			MappingLocalEntity();
			ReplicationRemoteEntity();

			if (frame_data.frame >= game_state.run_frame)
			{
				// 客户端比服务器慢(客户端卡了),直接复制服务器数据
				ReplicationLocalEntity();
				ReplicationLocalState();
			}
			else if(IsNeedRollback())
			{					
				RollbackLocalEntity();

				ReplicationLocalEntity();
				ReplicationLocalState();
			}

			ReplicationSingleEntity();
		}
	}

	void Finalize() override
	{

	}	
public:
	void MappingLocalEntity()
	{
		auto& components = serialization_manager.GetLocalEntityComponents<UniqueId>();

		auto view = registry.view<Local>(entt::exclude<UniqueId>);
		for (auto e : view)
		{
			auto& local = view.get<Local>(e);
			auto& it = components.find(local);

			if (it != components.end())
			{
				registry.emplace<UniqueId>(e, it->second);
			}
		}
	}

	void ReplicationLocalState()
	{
		auto& frame_data = serialization_manager.frame_data();

		auto& game_state = registry.ctx<GameState>();
		game_state.run_frame = frame_data.frame;

		auto& sync_manager = registry.ctx<SyncManager>();
		for (auto& item : sync_manager.sync_instances)
		{
			item.second.identifier = frame_data.local_entity_identifier;
		}

		{
			auto view = registry.view<TransformState, Transform>();
			for (auto e : view)
			{
				auto&[transform_state, transform] = view.get<TransformState, Transform>(e);
				transform_state.value[frame_data.frame % transform_state.value.size()] = transform;
			}
		}

		{
			auto view = registry.view<GameplayState, GameplayAttributeSet>();
			for (auto e : view)
			{
				auto&[gameplay_state, attribute_set] = view.get<GameplayState, GameplayAttributeSet>(e);
				gameplay_state.attribute_sets[frame_data.frame % gameplay_state.attribute_sets.size()] = attribute_set;

				auto& tag_count = registry.get<GameplayTagCountContainer>(e);
				gameplay_state.tag_counts[frame_data.frame % gameplay_state.tag_counts.size()] = tag_count;
			}
		}

		{
			auto& state = registry.ctx_or_set<LocalEntityState>();
			state.identifiers[frame_data.frame % state.identifiers.size()] = frame_data.local_entity_identifier;
			state.nums[frame_data.frame % state.nums.size()] = frame_data.local_entity_num;
		}
	}

	void ReplicationLocalEntity()
	{
		ReplicationLocalEntityComponents(LocalComponents{});
		ReplicationLocalEntityComponents(RemoteComponents{});
	}

	template<class ... Components>
	void ReplicationLocalEntityComponents(TypeList<Components...> = {})
	{
		(ReplicationLocalEntityComponent<Components>(), ...);
	}

	template<class Components>
	void ReplicationLocalEntityComponent()
	{
		auto& components = serialization_manager.GetLocalEntityComponents<Component>();
		for (auto& item : components)
		{
			entt::entity e = registry.get(item.first);
			if (e == entt::null)
			{
				e = registry.create();
				registry.emplace<Local>(e, item.first);
			}

			registry.emplace_or_replace<Component>(e, item.second);
		}
	}

public:
	void ReplicationRemoteEntity()
	{
		ReplicationRemoteEntityComponents(RemoteComponents{});
	}

	template<class ... Components>
	void ReplicationRemoteEntityComponents(TypeList<Components...> = {})
	{
		(ReplicationRemoteEntityComponent<Components>(), ...);
	}

	template<class Component>
	void ReplicationRemoteEntityComponent()
	{
		auto& components = serialization_manager.GetRemoteEntityComponents<Component>();

		for (auto& item : components)
		{
			entt::entity e = registry.get(item.first);

			if (e == entt::null)
			{
				e = registry.create();
				registry.emplace<UniqueId>(e, item.first);
			}

			//todo:可能还会有其他需要屏蔽情况?
			if (!registry.all_of<Local>(e))
			{
				registry.emplace_or_replace<Component>(e, item.second);
			}
		}
	}
public:
	void ReplicationSingleEntity()
	{
		ReplicationSingleEntityComponents(SingleComponents{});
	}

	template<class ... Components>
	void ReplicationSingleEntityComponents(TypeList<Components...> = {})
	{
		(ReplicationSingleEntityComponent<Components>(), ...);
	}

	template<class Components>
	void ReplicationSingleEntityComponent()
	{
		auto& components = serialization_manager.GetSingleEntityComponents<Component>();
		for (auto& component : components)
		{
			auto e = registry.create();
			registry.emplace<Component>(e, component);
		}
	}
	
public:
	bool CheckLocalState()
	{
		auto& frame_data = serialization_manager.frame_data();
		auto& state = registry.ctx<LocalEntityState>();
		if (frame_data.local_entity_identifier != state.identifiers[frame_data.frame % state.identifiers.size()])
		{
			INFO("CheckLocalState Identifiers Rollback");
			return false;
		}

		if (frame_data.local_entity_identifier != state.nums[frame_data.frame % state.nums.size()])
		{
			INFO("CheckLocalState Num Rollback");
			return false;
		}

		return true;
	}

	bool CheckLocalEntityComponents()
	{
		auto& frame_data = serialization_manager.frame_data();

		{
			auto& components = serialization_manager.GetLocalEntityComponents<Transform>();
			auto view = registry.view<Local,TransformState>();
			for (auto e : view)
			{
				auto& local = view.get<Local>(e);
				auto& transform_state = view.get<TransformState>(e);

				auto& it = components.find(local);
				if (it != components.end())
				{
					auto& server_transform = it->second;
					auto& local_transform = transform_state.value[frame_data.frame % transform_state.value.size()];

					if (glm::distance(server_transform.position, local_transform.position) >= 0.1f)
					{
						INFO("Position Rollback");
						return false;
					}
					
					auto& server_forward = server_transform.rotation * vec3(0, 0, 1);
					auto& local_forward = local_transform.rotation * vec3(0, 0, 1);

					// 旋转不一致应该没问题?是否需要判断
					float angle = glm::angle(server_forward, local_forward);
					if (glm::degrees(angle) > 2.f)
					{
						INFO("Rotation Rollback");
						return false;
					}
				}
				else
				{
					assert(false && "CheckLocalComponents!");
					return false;
				}
			}
		}

		{
			auto view = registry.view<Local, GameplayState>();
			for (auto e : view)
			{
				auto&[local, gameplay_state] = view.get<Local, GameplayState>(e);
				
				// tag
				{
					auto& components = serialization_manager.GetLocalEntityComponents<GameplayTagCountContainer>();
					auto it = components.find(local);
					if (it != components.end())
					{
						if (it->second != gameplay_state.tag_counts[frame_data.frame % gameplay_state.tag_counts.size()])
						{
							INFO("GameplayTagCountContainer Rollback");
							return false;
						}
					}
					else
					{
						assert(false && "CheckLocalComponents!");
						return false;
					}
				}

				// attribute
				{
					auto& components = serialization_manager.GetLocalEntityComponents<GameplayAttributeSet>();
					auto it = components.find(local);
					if (it != components.end())
					{
						auto& server_attr_set = it->second;
						auto& local_attr_set = gameplay_state.attribute_sets[frame_data.frame % gameplay_state.attribute_sets.size()];

						for (size_t i = 0; i < server_attr_set.value.size(); ++i)
						{
							if (server_attr_set.value[i].base_value - local_attr_set.value[i].base_value > 0.1f)
							{
								INFO("GameplayAttributeSet BaseValue Rollback");
								return false;
							}

							if (server_attr_set.value[i].current_value - local_attr_set.value[i].current_value > 0.1f)
							{
								INFO("GameplayTagCountContainer CurrentValue Rollback");
								return false;
							}
						}

					}
					else
					{
						assert(false && "CheckLocalComponents!");
						return false;
					}
				}
			}

		}

		return true;
	}

public:
	bool IsNeedRollback()
	{
		if (!CheckLocalState())
		{
			return true;
		}

		if (!CheckLocalEntityComponents())
		{
			return true;
		}

		return false;
	}

	void RollbackLocalEntity()
	{
		auto& frame_data = serialization_manager.frame_data();
		// 删除多创建的local entity
		auto& sync_manager = registry.ctx<SyncManager>();
		for (auto& item : sync_manager.sync_instances)
		{
			for (auto i = frame_data.local_entity_identifier; i < item.second.identifier; ++i)
			{
				entt::entity e = registry.get(Local(item.second.owner, i));
				if (e != entt::null)
				{
					registry.destroy(e);
				}
			}
		}
	}
};