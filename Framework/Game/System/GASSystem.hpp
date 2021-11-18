#pragma once

#include "Framework/Game/GAS/GameplayEffectManager.hpp"

#include "Framework/Game/Component/GAS/ApplyGameplayEffectList.hpp"
#include "Framework/Game/Component/GAS/GameplayEffectList.hpp"
#include "Framework/Game/Component/GAS/GameplayTags.hpp"

#include "Framework/Game/System.hpp"

struct GASSystem : public System
{
	entt::observer apply_ge_list{ registry,entt::collector.group<GameplayEffectList,GameplayTags,ApplyGameplayEffectList>()
		.update<ApplyGameplayEffectList>().where<GameplayEffectList>() };

	GASSystem(entt::registry& _registry) : System(_registry) { }
	~GASSystem() {}

	bool Initialize() override
	{
		sol::state& lua = registry.ctx<sol::state>();

		return true;
	}

	void FixedUpdate(fixed16 dt) override
	{
		
	}

	bool CheckApplicationTagRequirements(GameplayEffect& ge,GameplayTags& source_tags,GameplayTags& target_tags)
	{
		if (!source_tags.HasAll(ge.tag.application_tag_requirements.require_tags))
		{
			return false;
		}

		if (target_tags.HasAny(ge.tag.application_tag_requirements.ignore_tags))
		{
			return false;
		}

		return true;
	}

	void ApplyGameplayEffect()
	{
		sol::state& lua = registry.ctx<sol::state>();
		GameplayEffectManager& ge_manager = registry.ctx<GameplayEffectManager>();

		auto view = registry.view<ApplyGameplayEffectList,GameplayEffectList,GameplayTags>();
		if (apply_ge_list.size() > 0)
		{
			for (auto e : apply_ge_list)
			{
				auto&[apply_ge_list,ge_list,tags] = view.get<ApplyGameplayEffectList,GameplayEffectList,GameplayTags>(e);
				for (auto& ge_info : apply_ge_list.value)
				{
					GameplayEffectSpec spec(ge_manager.LoadGameplayEffect(ge_info.name), ge_info.level);
					
				}
			}
		}
	}

	void UpdateGameplayEffect()
	{

	}

	void Finalize() override
	{

	}
};
