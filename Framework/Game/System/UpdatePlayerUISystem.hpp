#pragma once

#include "Framework/Game/Locator.hpp"
#include "Framework/Game/UIService.hpp"
#include "Framework/Game/GAS/GameplayAbilityManager.hpp"
#include "Framework/Game/GAS/GameplayEffectManager.hpp"

#include "Framework/Game/Component/Local.hpp"
#include "Framework/Game/Component/GAS/GameplayAttributeSet.hpp"
#include "Framework/Game/Component/GAS/GameplayEffectSpec.hpp"
#include "Framework/Game/Component/GAS/ActiveGameplayAbilityList.hpp"

#include "Framework/Game/System.hpp"

#include "3rdparty/include/nlohmann/json.hpp"

struct CooldownInfo
{
	float remaining;
	float duration;
};

inline void to_json(nlohmann::json& j, const CooldownInfo& info)
{
	j["remaining"] = info.remaining;
	j["duration"] = info.duration;
}

struct UpdatePlayerUISystem : public System
{
	entt::observer updatelist{registry,entt::collector.group<Local, GameplayAttributeSet>().update<GameplayAttributeSet>().where<Local>() };

	UpdatePlayerUISystem(entt::registry& _registry)
		: System(_registry)
	{

	}

	~UpdatePlayerUISystem() {}

	bool Initialize() override
	{
		return true;
	}

	void UpdateAttribute()
	{
		auto& ui_service = registry.ctx<Locator>().Ref<UIService>();

		auto view = registry.view<Local, GameplayAttributeSet>();
		for (auto e : updatelist)
		{
			auto& attribute_set = view.get<GameplayAttributeSet>(e);
			ui_service.RefreshUI(
				attribute_set.value[static_cast<uint32_t>(GameplayAttributeType::Hp)].current_value,
				attribute_set.value[static_cast<uint32_t>(GameplayAttributeType::MaxHp)].current_value,
				attribute_set.value[static_cast<uint32_t>(GameplayAttributeType::Mp)].current_value,
				attribute_set.value[static_cast<uint32_t>(GameplayAttributeType::MaxMp)].current_value);
		}

		updatelist.clear();
	}

	void UpdateAbilityCooldown()
	{
		registry.view<ActiveGameplayAbilityList,UniqueId,Local>().each([this](entt::entity e,const ActiveGameplayAbilityList& list,const UniqueId& id, const Local&)
		{
			std::unordered_map<std::string, CooldownInfo> cooldowns;

			auto& ga_manager = registry.ctx<GameplayAbilityManager>();
			auto& ge_manager = registry.ctx<GameplayEffectManager>();

			for (auto& item : list.value)
			{
				// 遍历所有ge找到相关cooldowntas,
				// 记录最长的冷却时间
				auto view = registry.view<GameplayEffectSpec>();
				for (auto e : view)
				{
					auto& spec = view.get<GameplayEffectSpec>(e);
					if (spec.target == id)
					{
						auto& ge = ge_manager.LoadGameplayEffect(spec.effect);
						if (ge.gameplay_effect.duration_policy == GameplayEffect::DurationPolicy::Duration)
						{
							continue;
						}

						auto& ga = ga_manager.LoadGameplayAbility(item.first);

						if (spec.dynamic_granted_tags.HasAny(ga.cooldown_tags) || ge.tag.granted_tags.HasAny(ga.cooldown_tags))
						{
							auto it = cooldowns.find(item.first);
							if(it != cooldowns.end())
							{
								float remaining = ge.period.duration - spec.duration;
								if (remaining > it->second.remaining)
								{
									it->second.remaining = remaining;
									it->second.duration = ge.period.duration;
								}
							}
							else
							{
								cooldowns.emplace(item.first, CooldownInfo{ ge.period.duration - spec.duration,ge.period.duration });
							}
						}
					}
				}
			}

			nlohmann::json j(cooldowns);
			std::string json_info = j.dump();

			auto& ui_service = registry.ctx<Locator>().Ref<UIService>();
			ui_service.RefreshAbilityCooldown(json_info);
		});
	}

	void Update(fixed16 dt) override
	{
		UpdateAttribute();
		UpdateAbilityCooldown();
	}

	void Finalize() override
	{

	}
};