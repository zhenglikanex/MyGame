#pragma once

#include "Framework/Game/System.hpp"

#include "Framework/Game/Proto.hpp"
#include "Framework/Game/Locator.hpp"
#include "Framework/Game/NetworkService.hpp"

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
		FrameData frame_data;
		frame_data.frame = registry.ctx<GameState>().run_frame;
		
		auto view1 = registry.view<Transform>();
		for (auto e : view1)
		{
			auto& transform = view1.get<Transform>(e);
			frame_data.transforms.emplace(view1.get<UniqueId>(e), transform);
		}

		auto view2 = registry.view<AnimationClip>();
		for (auto e : view2)
		{
			auto& animation_clips = view2.get<AnimationClip>(e);
			frame_data.animation_clips.emplace(view2.get<UniqueId>(e),animation_clips);
		}

		auto view3 = registry.view<GameplayAttributeSet>();
		for (auto e : view3)
		{
			auto& attribute_sets = view3.get<GameplayAttributeSet>(e);
			frame_data.attribute_sets.emplace(view3.get<UniqueId>(e), attribute_sets);
		}

		auto view4 = registry.view<GameplayTags>();
		for (auto e : view4)
		{
			auto& tags = view4.get<GameplayTags>(e);
			frame_data.tags.emplace(view4.get<UniqueId>(e), tags);
		}

		auto view5 = registry.view<GameplayEffectSpec>();
		for (auto e : view5)
		{
			auto& spec = view5.get<GameplayEffectSpec>(e);
			frame_data.effect_specs.emplace(view5.get<UniqueId>(e), spec);
		}

		auto view6 = registry.view<GameplayAbilitySpec>();
		for (auto e : view6)
		{
			auto& spec = view6.get<GameplayAbilitySpec>(e);
			frame_data.ability_specs.emplace(view6.get<UniqueId>(e), spec);
		}

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