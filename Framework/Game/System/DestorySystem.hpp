#pragma once

#include "Framework/Game/Component/Destroy.hpp"
#include "Framework/Game/Component/GAS/GameplayAbilitySpec.hpp"
#include "Framework/Game/Component/GAS/GameplayEffectSpec.hpp"

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
				auto& game_state = registry.ctx<GameState>();
				INFO("frame {} DestoryGameplayAbility {}", game_state.run_frame,e);

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

	void DestoryGameplayEffect()
	{
		auto& ge_manager = registry.ctx<GameplayEffectManager>();
		auto view = registry.view<GameplayEffectSpec, Destroy>();
		for(auto e : view)
		{
			auto& spec = view.get<GameplayEffectSpec>(e);

			auto& ge =  ge_manager.LoadGameplayEffect(spec.effect);
			auto target = registry.get(spec.target);
			if (target != entt::null)
			{
				auto& ge = ge_manager.LoadGameplayEffect(spec.effect);
				registry.patch<GameplayTagCountContainer>(target, [&ge](GameplayTagCountContainer& tag_count)
				{
					tag_count.UpdateTagCount(ge.tag.granted_tags, -1);
				});

				// todo:移除技能等
				registry.emplace<Destroy>(e);
			}
		}
	}

	void DestoryEntity(const std::vector<entt::entity>& entities) 
	{
		if(!entities.empty())
		{
			auto& game_state = registry.ctx<GameState>();
			INFO("frame {} DestoryEntity size{}", entities.size(), game_state.run_frame);
		}
		
		registry.destroy(entities.begin(), entities.end());
	}

	void Finalize() override
	{

	}
};