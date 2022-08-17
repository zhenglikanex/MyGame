#pragma once

#include "Framework/Game/Component/Destroy.hpp"
#include "Framework/Game/Component/GAS/GameplayAbilitySpec.hpp"

#include "Framework/Game/System.hpp"


struct DestorySystem : System
{
public:
	explicit DestorySystem(Registry& _registry) : System(_registry)
	{
	}

	bool Initialize() override
	{
		return true;
	}

	void FixedUpdate(float dt) override
	{
		// 缓存下当帧销毁的entity,将在销毁中被销毁的关联对象移到下一帧销毁
		auto view = registry.view<Destroy>();
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
		auto& ga_manager = registry.ctx<GameplayAbilityManager>();
		auto view = registry.view<GameplayAbilitySpec,Destroy>();
		for (auto e : view)
		{
			auto& spec = view.get<GameplayAbilitySpec>(e);
			auto& ga = ga_manager.LoadGameplayAbility(spec.ability);

			auto source = registry.get(spec.source);
			if (source != entt::null)
			{
				registry.patch<GameplayTagCountContainer>(source,[&ga](GameplayTagCountContainer& tag_count)
					{
						tag_count.UpdateTagCount(ga.activation_owned_tags, -1);
					});
			}

			for (auto& item : spec.entity_manager)
			{
				auto e = registry.get(item.second);
				if (e != entt::null)
				{
					registry.emplace<Destroy>(e);
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