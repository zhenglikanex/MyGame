#pragma once

#include "Framework/Game/Component/Destory.hpp"
#include "Framework/Game/Component/GAS/GameplayAbilitySpec.hpp"

#include "Framework/Game/System.hpp"


struct DestorySystem : System
{
public:
	DestorySystem(Registry& _registry) : System(_registry)
	{
	}
	~DestorySystem() {}

	bool Initialize() override
	{
		return true;
	}

	void FixedUpdate(float dt) override
	{
		
		// 缓存下当帧销毁的entity,将在销毁中被销毁的关联对象移到下一帧销毁
		auto view = registry.view<Destory>();
		std::vector<entt::entity> entities;
		entities.reserve(view.size());
		for (auto e : view)
		{
			entities.push_back(e);
		}

		DestoryGameplayAbility();
		DestoryEntity(entities);
	}

	void DestoryGameplayAbility()
	{
		auto& map = registry.ctx<EntityByLocalMap>();
		auto view = registry.view<GameplayAbilitySpec,Destory>();
		for (auto e : view)
		{
			auto& spec = view.get<GameplayAbilitySpec>(e);
			for (auto& item : spec.entity_manager)
			{
				auto it = map.value.find(item.second);
				if (it != map.value.end())
				{
					registry.emplace<Destory>(it->second);
				}
			}
		}
	}

	void DestoryEntity(const std::vector<entt::entity>& entities) 
	{
		registry.destroy(entities.begin(), entities.end());
	}

	void Finalize() override
	{

	}
};