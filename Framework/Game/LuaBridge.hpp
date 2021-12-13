#pragma once

#include "Framework/Game/GAS/GameplayTag.hpp"
#include "Framework/Game/GAS/GameplayEffect.hpp"
#include "Framework/Game/GAS/GameplayAbility.hpp"

#include "Framework/Game/Component/DelayTask.hpp"
#include "Framework/Game/Component/GAS/GameplayAbilitySpec.hpp"

#include "Framework/Game/Utility/GameplayEffectUtility.hpp"
#include "Framework/Game/Utility/GameplayAbilityUtility.hpp"
#include "Framework/Game/Utility/GameplayEventUtility.hpp"

#include "3rdparty/include/entt/entt.hpp"

inline void RegisterGameplayTag(sol::state& lua)
{
	auto type = lua.new_usertype<GameplayTag>("GameplayTag", sol::constructors<GameplayTag(const std::string&), GameplayTag(const GameplayTag&)>());
	type["tag"] = &GameplayTag::tag;
	type["Contain"] = &GameplayTag::Contain;

	auto type2 = lua.new_usertype<GameplayTagArray>("GameplayTagArray");
	type2["Push"] = &GameplayTagArray::Push;
	type2["Clear"] = &GameplayTagArray::Clear;
	type2["Find"] = &GameplayTagArray::Find;
	type2["HasAll"] = &GameplayTagArray::HasAll;
	type2["HasAny"] = &GameplayTagArray::HasAny;
}

inline void RegisterGameplayEffect(sol::state& lua)
{
	auto type1 = lua.new_usertype<GameplayEffect::RequirementsTags>("GERequirementsTags");
	type1["require_tags"] = &GameplayEffect::RequirementsTags::require_tags;
	type1["ignore_tags"] = &GameplayEffect::RequirementsTags::ignore_tags;

	auto type2 = lua.new_usertype<GameplayEffect::Period>("GEPeriod");
	type2["period"] = &GameplayEffect::Period::period;
	type2["duration"] = &GameplayEffect::Period::duration;
	type2["periodic_inhibition_policy"] = &GameplayEffect::Period::periodic_inhibition_policy;

	auto type3 = lua.new_usertype<GameplayEffect::Application>("GEApplication");
	type3["chance_to_apply_to_target"] = &GameplayEffect::Application::chance_to_apply_to_target;

	auto type4 = lua.new_usertype<GameplayEffect::Immunity>("GEImmunity");
	type4["granted_application_immunity_tags"] = &GameplayEffect::Immunity::granted_application_immunity_tags;

	auto type5 = lua.new_usertype<GameplayEffect::Overflow>("GEOverflow");
	type5["overflow_effects"] = &GameplayEffect::Overflow::overflow_effects;
	type5["deny_overflow_application"] = &GameplayEffect::Overflow::deny_overflow_application;
	type5["clear_stack_on_overflow"] = &GameplayEffect::Overflow::clear_stack_on_overflow;

	auto type6 = lua.new_usertype<GameplayEffect::Stacking>("GEStacking");
	type6["stacking_type"] = &GameplayEffect::Stacking::stacking_type;
	type6["stack_limit_count"] = &GameplayEffect::Stacking::stack_limit_count;
	type6["stack_duration_refresh_policy"] = &GameplayEffect::Stacking::stack_duration_refresh_policy;
	type6["statck_period_reset_policy"] = &GameplayEffect::Stacking::statck_period_reset_policy;
	type6["statck_expiration_policy"] = &GameplayEffect::Stacking::statck_expiration_policy;

	auto type7 = lua.new_usertype<GameplayEffect::ModifierMagnitude>("GEModifierMagnitude");
	type7["calculation_type"] = &GameplayEffect::ModifierMagnitude::calculation_type;
	type7["value"] = &GameplayEffect::ModifierMagnitude::value;

	auto type8 = lua.new_usertype<GameplayEffect::Modifier>("GEModifier");
	type8["attribute"] = &GameplayEffect::Modifier::attribute;
	type8["modifier_op"] = &GameplayEffect::Modifier::modifier_op;
	type8["modifier_magnitude"] = &GameplayEffect::Modifier::modifier_magnitude;
	type8["source_tags"] = &GameplayEffect::Modifier::source_tags;
	type8["target_tags"] = &GameplayEffect::Modifier::target_tags;

	auto type9 = lua.new_usertype<GameplayEffect::Execution>("GEExecution");

	auto type10 = lua.new_usertype<GameplayEffect::GameplayEffectModifier>("GEGameplayEffectModifier");
	type10["duration_policy"] = &GameplayEffect::GameplayEffectModifier::duration_policy;
	type10["modifiers"] = &GameplayEffect::GameplayEffectModifier::modifiers;
	type10["executions"] = &GameplayEffect::GameplayEffectModifier::executions;

	auto type11 = lua.new_usertype<GameplayEffect::GrantedAbility>("GEGrantedAbility");
	type11["ability"] = &GameplayEffect::GrantedAbility::ability;
	type11["level"] = &GameplayEffect::GrantedAbility::level;
	type11["removal_policy"] = &GameplayEffect::GrantedAbility::removal_policy;

	auto type12 = lua.new_usertype<GameplayEffect::Display>("GEDisplay");
	type12["cues"] = &GameplayEffect::Display::cues;

	auto type13 = lua.new_usertype<GameplayEffect::Tag>("GETag");
	type13["gameplay_effect_asset_tags"] = &GameplayEffect::Tag::gameplay_effect_asset_tags;
	type13["granted_tags"] = &GameplayEffect::Tag::granted_tags;
	type13["ongoing_tag_requirements"] = &GameplayEffect::Tag::ongoing_tag_requirements;
	type13["application_tag_requirements"] = &GameplayEffect::Tag::application_tag_requirements;
	type13["removal_tag_requirements"] = &GameplayEffect::Tag::removal_tag_requirements;
	type13["remove_gameplay_effects_with_tags"] = &GameplayEffect::Tag::remove_gameplay_effects_with_tags;

	auto type14 = lua.new_usertype<GameplayEffect>("GameplayEffect");
	type14["period"] = &GameplayEffect::period;
	type14["application"] = &GameplayEffect::application;
	type14["immunity"] = &GameplayEffect::immunity;
	type14["overflow"] = &GameplayEffect::overflow;
	type14["stacking"] = &GameplayEffect::stacking;
	type14["gameplay_effect"] = &GameplayEffect::gameplay_effect;
	type14["granted_abilities"] = &GameplayEffect::granted_abilities;
	type14["display"] = &GameplayEffect::display;
	type14["tag"] = &GameplayEffect::tag;
}

inline void RegisterGameplayAbility(sol::state& lua)
{
	auto type = lua.new_usertype<GameplayAbility>("GameplayAbility");
	type["cooldown_gameplay_effect"] = &GameplayAbility::cooldown_gameplay_effect;
	type["cost_gameplay_effect"] = &GameplayAbility::cooldown_gameplay_effect;
	type["ability_tags"] = &GameplayAbility::ability_tags;
	type["cancel_ability"] = &GameplayAbility::cancel_ability;
	type["block_abilites_with_tag"] = &GameplayAbility::block_abilites_with_tag;
	type["activation_owned_tags"] = &GameplayAbility::activation_owned_tags;
	type["activation_required_tags"] = &GameplayAbility::activation_required_tags;
	type["activation_blocked_tags"] = &GameplayAbility::activation_blocked_tags;
	type["source_required_tags"] = &GameplayAbility::source_required_tags;
	type["source_blocked_tags"] = &GameplayAbility::source_blocked_tags;
	type["target_required_tags"] = &GameplayAbility::target_required_tags;
	type["target_blocked_tags"] = &GameplayAbility::target_blocked_tags;
	type["script"] = &GameplayAbility::script;
}

inline void RegisterGameplayAbilitySpec(sol::state& lua)
{
	auto type = lua.new_usertype<GameplayAbilitySpec>("GameplayAbilitySpec");
	type["ga"] = &GameplayAbilitySpec::ga;
	type["script"] = &GameplayAbilitySpec::script;
	type["level"] = &GameplayAbilitySpec::level;
	type["source"] = &GameplayAbilitySpec::source;
	type["target"] = &GameplayAbilitySpec::target;
}

inline void RegisterLua(entt::registry& registry,sol::state& lua)
{
	RegisterGameplayTag(lua);
	RegisterGameplayEffect(lua);
	RegisterGameplayAbility(lua);
	RegisterGameplayAbilitySpec(lua);

	auto delay_task1 = [&registry](entt::entity entity, float time, const sol::function& func) {
		auto& delay_task = registry.ctx_or_set<DelayTask>();
		delay_task.entity_by_events[entity].emplace_back(time, func);
	};

	auto delay_task2 = [&registry](float time, const sol::function& func) {
		auto& delay_task = registry.ctx_or_set<DelayTask>();
	};
	lua["DelayTask"] = sol::overload(delay_task1,delay_task2);

	lua["ApplyGameplayEffectToTarget"] = [&registry](entt::entity source, std::string_view effect, uint32_t level, entt::entity target)
	{
		GameplayEffectUtility::ApplyGameEffectToTarget(registry, source, effect, level, target);
	};

	lua["TryGameplayAbility"] = [&registry](entt::entity source, std::string_view ability, entt::entity target)
	{
		GameplayAbilityUtility::TryGameplayAbility(registry, source, ability, target);
	};

	lua["DestoryEntity"] = [&registry](entt::entity e)
	{
		registry.destroy(e);
	};

	lua["ReceiveGameplayEvent"] = [&registry](entt::entity e, std::string_view tag, const sol::function& func)
	{
		GameplayEventUtility::ReceiveGameplayEvent(registry, e,GameplayTag(tag), func);
	};

	lua["SendGameplayEvent"] = [&registry](std::string_view tag, void* data)
	{
		GameplayEventUtility::SendGameplayEvent(registry, GameplayTag(tag), data);
	};

	lua["RemoveReceiveGameplayEvent"] = [&registry](entt::entity e,std::string_view tag)
	{
		GameplayEventUtility::RemoveReceiveGameplayEvent(registry, e, GameplayTag(tag));
	};
}