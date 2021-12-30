#pragma once

#include "Framework/Game/System.hpp"

#include "Framework/Game/Proto.hpp"
#include "Framework/Game/Locator.hpp"
#include "Framework/Game/NetworkService.hpp"

#include "Framework/Game/Component/Player.hpp"
#include "Framework/Game/Component/PlayerManager.hpp"

#include "Server/BattleActor/ServerNetworkService.hpp"

using namespace std;
using namespace std::chrono;

struct ServerSyncSystem : public System
{
	kanex::Buffer buffer;

	ServerSyncSystem(entt::registry& _registry)
		: System(_registry)
		, buffer(1024)
	{
		
	}

	~ServerSyncSystem()
	{

	}

	bool Initialize() override
	{
		return true;
	}

	void FixedUpdate(fixed16 dt) override
	{
		auto& player_manager = registry.ctx<PlayerManager>();
		INFO("server sync{}",duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());
		for (auto item : player_manager.players)
		{
			auto& player_data = item.second;

			FrameData frame_data;
			frame_data.frame = registry.ctx<GameState>().run_frame;
			frame_data.local_id = player_data.local_id_generator;

			INFO("frame {}", frame_data.frame);

			auto view = registry.view<UniqueId, LocalId, Player>();
			for (auto e : view)
			{
				auto& [unique_id, local_id, player] = view.get<UniqueId, LocalId, Player>(e);
				if (player.id == player_data.id)
				{
					frame_data.unique_ids.emplace(local_id, unique_id);
				}
			}

			auto actor_asset_view = registry.view<UniqueId, ActorAsset>();
			for (auto e : actor_asset_view)
			{
				auto& [unique_id,asset] = actor_asset_view.get<UniqueId, ActorAsset>(e);
				frame_data.actor_assets.emplace(unique_id, asset);
			}

			auto view1 = registry.view<UniqueId,Transform>();
			for (auto e : view1)
			{
				auto& [unique_id,transform] = view1.get<UniqueId,Transform>(e);
				frame_data.transforms.emplace(unique_id, transform);

				INFO("id{} player{} x {} y {} z {} ", unique_id.value, registry.get<Player>(e).id, transform.position.x, transform.position.y, transform.position.z);
				
			}
			INFO("-----------------------------");

			auto view2 = registry.view<UniqueId,AnimationClip>();
			for (auto e : view2)
			{
				auto& [unique_id,animation_clip] = view2.get<UniqueId,AnimationClip>(e);
				frame_data.animation_clips.emplace(unique_id, animation_clip);
			}

			auto view3 = registry.view<UniqueId,GameplayAttributeSet>();
			for (auto e : view3)
			{
				auto& [unique_id,attribute_set] = view3.get<UniqueId,GameplayAttributeSet>(e);
				frame_data.attribute_sets.emplace(unique_id, attribute_set);
			}

			auto view4 = registry.view<UniqueId,GameplayTags>();
			for (auto e : view4)
			{
				auto& [unique_id,tags] = view4.get<UniqueId,GameplayTags>(e);
				frame_data.tags.emplace(unique_id, tags);
			}

			auto view5 = registry.view<UniqueId,GameplayEffectSpec>();
			for (auto e : view5)
			{
				auto& [unique_id,spec] = view5.get<UniqueId,GameplayEffectSpec>(e);
				frame_data.effect_specs.emplace(unique_id, spec);
			}

			auto view6 = registry.view<UniqueId,GameplayAbilitySpec>();
			for (auto e : view6)
			{
				auto& [unique_id,spec] = view6.get<UniqueId,GameplayAbilitySpec>(e);
				frame_data.ability_specs.emplace(unique_id, spec);
			}

			kanex::BinaryStream stream(buffer);
			kanex::BinaryOutputArchive oar(stream);
			oar(frame_data);

			std::vector<uint8_t> data(buffer.Size());
			buffer.Read(data.data(), data.size());

			buffer.Clear();

			auto& network_service = dynamic_cast<ServerNetworkService&>(registry.ctx<Locator>().Ref<NetworkService>());
			network_service.Send(player_data.id, "sync_frame_data", std::move(data));
		}
	}

	void Finalize() override
	{

	}
};