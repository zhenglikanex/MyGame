#pragma once

#include "Framework/Game/Component/AnimationAsset.hpp"
#include "Framework/Game/Component/Animation.hpp"
#include "Framework/Game/Component/AnimationClip.hpp"

#include "Framework/Game/System.hpp"

#include "Framework/Game/Data/AnimationConfig.hpp"

struct CreateAnimationSystem : System
{
	const std::string path = "Config/Anim/";
	std::unordered_map<std::string, AnimationInfo> animation_infos;

	explicit CreateAnimationSystem(Registry& registry) 
		: System(registry)
	{

	}

	bool Initialize() override
	{
		//todo:预加载一下
		GetAnimationInfo("SuperHero.json");
		return true;
	}

	void FixedUpdate(float dt) override
	{
		auto view = registry.view<AnimationAsset>(entt::exclude_t<Animation>{});
		for (auto e : view)
		{
			auto& asset = view.get<AnimationAsset>(e);
			auto& animation_info = GetAnimationInfo(asset.value);
			registry.emplace<Animation>(e, &animation_info);
			if (!registry.all_of<AnimationClip>(e) && animation_info.default_clip != "")
			{
				registry.emplace<AnimationClip>(e, animation_info.default_clip);
			}
		}
	}

	void Finalize() override
	{
		
	}

	// todo:
	// 应该单独拿出来做个Manager，方便预加载资源，偷个懒
	//1. 如果作为一个单例则服务器会有并发问题(可以考虑无锁实现)
	//2. 如果作为一个实例实现则会存在多份资源
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