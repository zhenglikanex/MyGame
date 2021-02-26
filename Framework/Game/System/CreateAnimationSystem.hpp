#pragma once

#include "Framework/Game/Component/AnimationAsset.hpp"
#include "Framework/Game/Component/Animation.hpp"

#include "Framework/Game/System.hpp"

#include "Framework/Game/Data/AnimationConfig.hpp"

struct CreateAnimationSystem : System
{
	const std::string path = "Config/Anim/";
	std::unordered_map<std::string, AnimationInfo> animation_infos;

	CreateAnimationSystem(entt::registry& registry) 
		: System(registry)
	{

	}

	~CreateAnimationSystem()
	{

	}

	bool Initialize() override
	{
		return true;
	}

	void Update(fixed16 dt)
	{
		auto view = registry.view<AnimationAsset>(entt::exclude_t<Animation>{});
		for (auto e : view)
		{
			auto& asset = view.get<AnimationAsset>(e);
			auto& animation_info = GetAnimationInfo(asset.value);
			registry.emplace<Animation>(e, &animation_info);
		}
	}

	void Finalize()
	{
		
	}

	const AnimationInfo& GetAnimationInfo(const std::string& name)
	{
		auto iter = animation_infos.find(name);
		if (iter != animation_infos.end())
		{
			return iter->second;
		}
		else
		{
			auto& locator = registry.ctx<Locator>();
			auto content = locator.Ref<FileService>().OpenFile(path + name);

			assert(content != "" && "not anim info");

			nlohmann::json j = nlohmann::json::parse(content);
			animation_infos.emplace(name,j.get<AnimationInfo>());

			return animation_infos[name];
		}
	}
};