#pragma once

#include <any>

#include "Framework/Game/Registry.hpp"

#include "Framework/Game/Component/GameState.hpp"
#include "Framework/Game/Component/Remote.hpp"
#include "Framework/Game/Component/ActorAsset.hpp"
#include "Framework/Game/Component/Local.hpp"
#include "Framework/Game/Component/Player.hpp"
#include "Framework/Game/Component/Transform.hpp"
#include "Framework/Game/Component/AnimationClip.hpp"
#include "Framework/Game/Component/Movement.hpp"
#include "Framework/Game/Component/ViewAsset.hpp"
#include "Framework/Game/Component/Destory.hpp"
#include "Framework/Game/Component/GAS/GameplayAttributeSet.hpp"
#include "Framework/Game/Component/GAS/GameplayEffectSpec.hpp"
#include "Framework/Game/Component/GAS/GameplayAbilitySpec.hpp"
#include "Framework/Game/Component/GAS/GameplayWaitTask.hpp"
#include "Framework/Game/Component/GAS/GameplayPlayAnimAndWaitTask.hpp"
#include "Framework/Game/Component/GAS/GameplayInputTask.hpp"
#include "Framework/Game/Component/GAS/GameplayHitTestTask.hpp"
#include "Framework/Game/Component/GAS/ModifierAttribute.hpp"

#include "Kanex.hpp"

template<class ... Components>
struct TypeList
{};

using LocalComponents = TypeList
<
	Remote,
	MovementForward,
	CharacterMovement,
	InfiniteModifierAttributeSet,
	GameplayWaitTask,
	GameplayPlayAnimAndWaitTask,
	GameplayInputTask,
	GameplayHitTestTask,
	IgnoreGravity
>;

using RemoteComponents = TypeList
<
	Player,
	ActorAsset,
	ViewAsset,
	Transform,
	AnimationClip,
	GameplayAttributeSet,
	GameplayTagCountContainer,
	GameplayAbilitySpec,
	GameplayEffectSpec,
	Destory
>;

using SingleComponents = TypeList
<
	ModifierAttribute
>;

struct FrameData
{
	uint32_t frame;
	uint32_t local_entity_identifier;
	uint32_t local_entity_num;
};

NON_MEMEBER_BAR(FrameData, obj.frame, obj.local_entity_identifier, obj.local_entity_num);

class SerializationManager
{
public:
	SerializationManager()
		: buffer_(0)
		, stream_(buffer_)
		, iar_(stream_)
		, oar_(stream_)
	{

	}
public:
	void Clear()
	{
		buffer_.Clear();
	}

	void Serialize(const Registry& registry,const SyncInstance& sync_instance)
	{
		SerializeCommon(registry);
		SerializeInstance(registry, sync_instance);
	}

	void SerializeCommon(const Registry& registry)
	{
		SerializeRemoteEntity(registry);
		SerializeSingleEntity(registry);
	}

	void SerializeInstance(const Registry& registry, const SyncInstance& sync_instance)
	{
		SerializeFrameData(registry, sync_instance);
		SerializeLocalEntity(registry, sync_instance);
	}

	void Deserialize(std::vector<uint8_t> data)
	{
		buffer_ = kanex::Buffer(std::move(data));

		DeserializeRemoteEntity();
		DeserializeSingleEntity();

		DeserializeFrameData();
		DeserializeLocalEntity();
	}
private:
	void SerializeFrameData(const Registry& registry, const SyncInstance& sync_instance)
	{
		auto& game_state = registry.ctx<GameState>();
		frame_data_.frame = game_state.run_frame;
		frame_data_.local_entity_identifier = sync_instance.identifier;
		
		frame_data_.local_entity_num = 0;
		auto view = registry.view<Local>();
		for (auto e : view)
		{
			auto& local = view.get<Local>(e);
			if (local.owner == sync_instance.owner)
			{
				++frame_data_.local_entity_num;
			}
		}

		oar_(frame_data_);
	}

	void SerializeLocalEntity(const Registry& registry, const SyncInstance& sync_instance)
	{
		SerializeLocalEntityComponent(registry, sync_instance, LocalComponents{});
		SerializeLocalEntityComponent(registry, sync_instance, RemoteComponents{});
	}

	template<class ... Components>
	void SerializeLocalEntityComponent(const Registry& registry, const SyncInstance& sync_instance, TypeList<Components...> = {})
	{
		(SerializeLocalEntityComponent<Components>(registry, sync_instance), ...);
	}

	template<class Component>
	void SerializeLocalEntityComponent(const Registry& registry, const SyncInstance& sync_instance)
	{
		auto& components = GetLocalEntityComponents<Component>();
		components.clear();

		auto view = registry.view<Local, Component>();
		for (auto e : view)
		{
			auto&[local, component] = view.get<Local, Component>(e);
			if (local.owner == sync_instance.owner)
			{
				components.emplace(local, component);
			}
		}

		oar_(components);
	}

	void SerializeRemoteEntity(const Registry& registry)
	{
		SerializeRemoteEntityComponent(registry,RemoteComponents{});
	}

	template<class ... Components>
	void SerializeRemoteEntityComponent(const Registry& registry,TypeList<Components...> = {})
	{
		(SerializeRemoteEntityComponent<Components>(registry), ...);
	}

	template<class Component>
	void SerializeRemoteEntityComponent(const Registry& registry)
	{
		auto& components = GetRemoteEntityComponents<Component>();
		components.clear();

		auto view = registry.view<Remote,Component>();
		for (auto e : view)
		{
			auto&[remote, component] = view.get<Remote,Component>(e);
			components.emplace(remote, component);
		}

		oar_(components);
	}

	void SerializeSingleEntity(const Registry& registry)
	{
		SerializeSingleEntityComponent(registry,SingleComponents{});
	}

	template<class ... Components>
	void SerializeSingleEntityComponent(const Registry& registry,TypeList<Components...> = {})
	{
		(GetSingleEntityComponents<Components>(registry), ...);
	}

	template<class Component>
	void SerializeSingleEntityComponent(const Registry& registry)
	{
		auto& components = GetSingleEntityComponents<Component>();
		components.clear();

		auto view = registry.view<Component>();
		for (auto e : view)
		{
			auto& component = view.get<Component>(e);
			components.emplace_back(component);
		}

		oar_(components);
	}
private:
	void DeserializeFrameData()
	{
		iar_(frame_data_);
	}

	void DeserializeLocalEntity()
	{
		DeserializeLocalEntityComponent(LocalComponents{});
		DeserializeLocalEntityComponent(RemoteComponents{});
	}

	template<class ... Components>
	void DeserializeLocalEntityComponent(TypeList<Components...> = {})
	{
		(DeserializeLocalEntityComponent<Components>(), ...);
	}

	template<class Component>
	void DeserializeLocalEntityComponent()
	{
		auto& components = GetLocalEntityComponents<Component>();
		components.clear();

		iar_(components);
	}

	void DeserializeRemoteEntity()
	{
		DeserializeRemoteEntityComponent(RemoteComponents{});
	}

	template<class ... Components>
	void DeserializeRemoteEntityComponent(TypeList<Components...> = {})
	{
		(DeserializeRemoteEntityComponent<Components>(), ...);
	}

	template<class Component>
	void DeserializeRemoteEntityComponent()
	{
		auto& components = GetRemoteEntityComponents<Component>();
		components.clear();

		iar_(components);
	}

	void DeserializeSingleEntity()
	{
		DeserializeSingleEntityComponent(SingleComponents{});
	}

	template<class ... Components>
	void DeserializeSingleEntityComponent(TypeList<Components...> = {})
	{
		(GetSingleEntityComponents<Components>(), ...);
	}

	template<class Component>
	void DeserializeSingleEntityComponent()
	{
		auto& components = GetSingleEntityComponents<Component>();
		components.clear();

		iar_(components);
	}
public:
	const FrameData& frame_data() const { return frame_data_; }
	kanex::Buffer& buffer() { return buffer_; }
	
	template<class Component>
	std::unordered_map<Local, Component>& GetLocalEntityComponents()
	{
		auto it = local_component_enities_.find(entt::type_hash<Component>::value());
		if (it == local_component_enities_.end())
		{
			local_component_enities_.emplace(entt::type_hash<Component>::value(), std::unordered_map<Local, Component>{});
		}

		return local_component_enities_[entt::type_hash<Component>::value()];
	}

	template<class Component>
	std::unordered_map<Remote, Component>& GetRemoteEntityComponents()
	{
		auto it = remote_component_enities_.find(entt::type_hash<Component>::value());
		if (it == remote_component_enities_.end())
		{
			remote_component_enities_.emplace(entt::type_hash<Component>::value(), std::unordered_map<Remote, Component>{});
		}

		return remote_component_enities_[entt::type_hash<Component>::value()];
	}

	template<class Component>
	std::vector<Component>& GetSingleEntityComponents()
	{
		auto it = single_component_enities_.find(entt::type_hash<Component>::value());
		if (it == single_component_enities_.end())
		{
			single_component_enities_.emplace(entt::type_hash<Component>::value(), std::vector<Component>{});
		}

		return single_component_enities_[entt::type_hash<Component>::value()];
	}
private:
	kanex::Buffer buffer_;
	kanex::BinaryStream stream_;
	kanex::BinaryInputArchive iar_;
	kanex::BinaryOutputArchive oar_;

	FrameData frame_data_;

	//所有local entity的component数据,存储RemoteComponent和LocalComponent
	std::unordered_map<uint32_t, std::any> local_component_enities_;

	//所有remote entity的component数据,只存储RemoteComponent
	std::unordered_map<uint32_t, std::any> remote_component_enities_;

	//存储SingleComponent
	std::unordered_map<uint32_t, std::any> single_component_enities_;
};