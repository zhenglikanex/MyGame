#pragma once

#include "Framework/Game/Locator.hpp"
#include "Framework/Game/FileService.hpp"
#include "Framework/Game/DebugService.hpp"
#include "Framework/Game/Registry.hpp"

#include "Framework/Game/GAS/GameplayTag.hpp"
#include "Framework/Game/GAS/GameplayEffect.hpp"
#include "Framework/Game/GAS/GameplayAbility.hpp"

#include "Framework/Game/Component/View.hpp"
#include "Framework/Game/Component/ViewAsset.hpp"
#include "Framework/Game/Component/Movement.hpp"
#include "Framework/Game/Component/TransformState.hpp"
#include "Framework/Game/Component/Player.hpp"
#include "Framework/Game/Component/Collider.hpp"
#include "Framework/Game/Component/ParentAnimBone.hpp"
#include "Framework/Game/Component/Weapon.hpp"
#include "Framework/Game/Component/Destroy.hpp"
#include "Framework/Game/Component/Transform.hpp"
#include "Framework/Game/Component/SyncManager.hpp"
#include "Framework/Game/Component/CollisionReceiver.hpp"
#include "Framework/Game/Component/AnimationClip.hpp"
#include "Framework/Game/Component/GAS/GameplayAbilitySpec.hpp"
#include "Framework/Game/Component/GAS/GameplayWaitTask.hpp"
#include "Framework/Game/Component/GAS/GameplayPlayAnimAndWaitTask.hpp"
#include "Framework/Game/Component/GAS/GameplayInputTask.hpp"
#include "Framework/Game/Component/GAS/GameplayEvent.hpp"
#include "Framework/Game/Component/GAS/GameplayHitTestTask.hpp"

#include "Framework/Game/Utility/GameplayEffectUtility.hpp"
#include "Framework/Game/Utility/GameplayAbilityUtility.hpp"
#include "Framework/Game/Utility/GameplayEventUtility.hpp"
#include "Framework/Game/Utility/PhysicsUtility.hpp"

#include "3rdparty/include/entt/entt.hpp"

inline void RegisterLocal(sol::state& lua)
{
	auto type = lua.new_usertype<Local>("Local");
	type["owner"] = &Local::owner;
	type["id"] = &Local::id;
}

inline void RegisterVec3(sol::state& lua)
{
	auto type = lua.new_usertype<vec3>("vec3", sol::constructors<vec3(float,float,float)>());
	type["x"] = &vec3::x;
	type["y"] = &vec3::y;
	type["z"] = &vec3::z;
}

inline void RegisterQuat(sol::state& lua)
{
	auto type = lua.new_usertype<quat>("quat", sol::constructors<vec3(float, float, float, float)>());
	type["x"] = &quat::x;
	type["y"] = &quat::y;
	type["z"] = &quat::z;
	type["w"] = &quat::w;
}

inline void RegisterMovementForward(sol::state& lua)
{
	auto type = lua.new_usertype<MovementForward>("MovementForward", sol::constructors<MovementForward(), MovementForward(const vec3&)>());
	type["value"] = &MovementForward::value;
}

inline void RegisterMovement(sol::state& lua)
{
	auto type = lua.new_usertype<Movement>("Movement",sol::constructors<Movement(),Movement(const vec3&,const vec3&)>());
	type["velocity"] = &Movement::velocity;
	type["angular_velocity"] = &Movement::angular_velocity;
}

inline void RegisterTransform(sol::state& lua)
{
	auto type = lua.new_usertype<Transform>("Transform");
	type["position"] = &Transform::position;
	type["rotation"] = &Transform::rotation;
}

inline void RegisterWeapon(sol::state& lua)
{
	auto type = lua.new_usertype<Weapon>("Weapon");
	type["collider"] = &Weapon::collider;
}

inline void RegisterGameplayTag(sol::state& lua)
{
	auto type = lua.new_usertype<GameplayTag>("GameplayTag", sol::constructors<GameplayTag(uint64_t,uint64_t), GameplayTag(const GameplayTag&)>());
	type["value"] = &GameplayTag::value;
	type["Matching"] = &GameplayTag::Matching;
	type["MatchingExact"] = &GameplayTag::MatchingExact;
}

inline void RegisterGameplayTagArray(sol::state& lua)
{
	auto type = lua.new_usertype<GameplayTagArray>("GameplayTagArray");
	type["Push"] = &GameplayTagArray::Push;
	type["Clear"] = &GameplayTagArray::Clear;
	type["HasTag"] = &GameplayTagArray::HasTag;
	type["HasTagExact"] = &GameplayTagArray::HasTagExact;
	type["HasAll"] = &GameplayTagArray::HasAll;
	type["HasAllExact"] = &GameplayTagArray::HasAllExact;
	type["HasAny"] = &GameplayTagArray::HasAny;
	type["HasAnyExact"] = &GameplayTagArray::HasAnyExact;
}

inline void RegisterGameplayTagCountContainer(sol::state& lua)
{
	auto type = lua.new_usertype<GameplayTagCountContainer>("GameplayTagCountContainer");
	type["GetExplicitGameplayTags"] = &GameplayTagCountContainer::GetExplicitGameplayTags;
	type["GetTagCount"] = &GameplayTagCountContainer::GetTagCount;
	type["HasTag"] = &GameplayTagCountContainer::HasTag;
	type["HasTagExact"] = &GameplayTagCountContainer::HasTagExact;
	type["HasAll"] = &GameplayTagCountContainer::HasAll;
	type["HasAny"] = &GameplayTagCountContainer::HasAny;
	type["SetTagCount"] = &GameplayTagCountContainer::SetTagCount;

	//todo:
	//auto UpdateTagCount1 = std::bind((void(GameplayTagCountContainer::*)(const GameplayTag&, int32_t)) & GameplayTagCountContainer::UpdateTagCount, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	//auto UpdateTagCount2 = std::bind((void(GameplayTagCountContainer::*)(const GameplayTagArray&, int32_t)) & GameplayTagCountContainer::UpdateTagCount, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	type["UpdateTagCount"] = (void(GameplayTagCountContainer::*)(const GameplayTag&, int32_t)) & GameplayTagCountContainer::UpdateTagCount;
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
	type9["class_name"] = &GameplayEffect::Execution::class_name;
	type9["modifiers"] = &GameplayEffect::Execution::modifiers;

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
	type["cost_gameplay_effect"] = &GameplayAbility::cost_gameplay_effect;
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
	type["cooldown_duration"] = &GameplayAbility::cooldown_duration;
	type["cooldown_tags"] = &GameplayAbility::cooldown_tags;

	type["script"] = &GameplayAbility::script;
}

inline void RegisterGameplayAbilitySpec(sol::state& lua)
{
	auto type = lua.new_usertype<GameplayAbilitySpec>("GameplayAbilitySpec");
	type["ability"] = &GameplayAbilitySpec::ability;
	type["level"] = &GameplayAbilitySpec::level;
	type["source"] = &GameplayAbilitySpec::source;
	type["target"] = &GameplayAbilitySpec::target;
	type["activation"] = &GameplayAbilitySpec::activation;
	type["entity_manager"] = &GameplayAbilitySpec::entity_manager;
}

inline void RegisterLua(Registry& registry,sol::state& lua)
{
	auto& file_service = registry.ctx<Locator>().Ref<FileService>();
	lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string, sol::lib::math, sol::lib::table, sol::lib::debug, sol::lib::os);

	RegisterLocal(lua);
	RegisterVec3(lua);
	RegisterQuat(lua);
	RegisterMovementForward(lua);
	RegisterMovement(lua);
	RegisterTransform(lua);
	RegisterWeapon(lua);
	RegisterGameplayTag(lua);
	RegisterGameplayTagArray(lua);
	RegisterGameplayTagCountContainer(lua);
	RegisterGameplayEffect(lua);
	RegisterGameplayAbility(lua);
	RegisterGameplayAbilitySpec(lua);

	lua["load_path"] = file_service.cur_path() + "Lua/Script/?.lua";
	lua.script_file(file_service.cur_path() + "Lua/Script/main.lua", sol::script_default_on_error);
	sol::function::set_default_handler(lua["__G__TRACKBACK__"]);

	// todo:≤‚ ‘”√
	lua["TestSphereCollider"] = [&registry](float raduis)
	{
		Collider colldier(Geometry(Sphere(raduis)),glm::identity<mat4>(),true,RigidType::kKinematic);
		return colldier;
	};

	lua["EnttNull"] = static_cast<entt::entity>(entt::null);

	lua["ApplyGameplayEffectToTarget"] = [&registry](entt::entity source, std::string_view effect, uint32_t level, entt::entity target)
	{
		GameplayEffectUtility::ApplyGameEffectToTarget(registry, source, effect, level, target);
	};

	lua["GameplayWait"] = [&registry](entt::entity ability, float time, std::string_view completed)
	{
		auto& task = registry.get_or_emplace<GameplayWaitTask>(ability);
		task.value.emplace_back(time, completed);
	};

	lua["PlayAnimation"] = [&registry](Remote id, std::string_view name,float time = 0)
	{
		auto e = registry.get(id);
		registry.emplace_or_replace<AnimationClip>(e, name, time);
	};

	lua["GameplayPlayAnimAndWait"] = [&registry](entt::entity ability,std::string_view name,std::string_view completed,std::string_view canceled)
	{
		auto& task = registry.get_or_emplace<GameplayPlayAnimAndWaitTask>(ability);
		task.value.emplace_back(name, completed, canceled);
	};

	lua["GameplayInput"] = [&registry](entt::entity ability, std::string_view key, std::string_view pressed, std::string_view released)
	{
		auto& task = registry.get_or_emplace<GameplayInputTask>(ability);
		task.value.emplace(key,GameplayInputSpec(key, pressed, released));
	};

	lua["RemoveGameplayInput"] = [&registry](entt::entity ability, std::string_view key)
	{
		auto& task = registry.get_or_emplace<GameplayInputTask>(ability);
		task.value.erase(key.data());
	};

	lua["GameplayWaitEvent"] = [&registry](entt::entity ability,const GameplayTag& tag,std::string_view handler)
	{
		auto& task = registry.get_or_emplace<GameplayWaitEventTask>(ability);
		task.value.emplace(tag, GameplayWaitEventSpec(tag,handler));
	};

	lua["RemoveGameplayWaitEvent"] = [&registry](entt::entity ability, const GameplayTag& tag)
	{
		auto& task = registry.get_or_emplace<GameplayWaitEventTask>(ability);
		task.value.erase(tag);
	};

	lua["GameplayHitTest"] = [&registry](entt::entity ability, GameplayHitType type, const Collider& collider, const Transform& transform,std::string_view on_hit)
	{
		auto& task = registry.get_or_emplace<GameplayHitTestTask>(ability);
		task.value.emplace_back(type, collider, transform, on_hit);
	};

	lua["CreateAnimBone"] = [&registry](Remote target, std::string_view bone)
	{
		entt::entity target_entity = registry.get(target);
		auto e = registry.create_local(target_entity);
		if (auto transform = registry.try_get<Transform>(target_entity))
		{
			registry.emplace<Transform>(e, *transform);
		}
		else
		{
			registry.emplace<Transform>(e);
		}
		registry.emplace<ParentAnimBone>(e, target, bone);

		return registry.get<Local>(e);
	};

	lua["GetWeapon"] = [&registry](Remote target)
	{
		Weapon* weapon = nullptr;
		entt::entity e = registry.get(target);
		if (e != entt::null)
		{
			weapon = registry.try_get<Weapon>(e);
		}

		return weapon;
	};

	lua["GetUniqueId"] = [&registry](entt::entity e)
	{
		return registry.try_get<Remote>(e);
	};
	
	auto destory1 = [&registry](entt::entity entity) 
	{
		if (!registry.all_of<Destroy>(entity))
		{
			registry.emplace<Destroy>(entity);
		}
	};

	auto destory2 = [&registry](Local local) 
	{
		entt::entity e = registry.get(local);
		if (e != entt::null)
		{
			if (!registry.all_of<Destroy>(e))
			{
				registry.emplace<Destroy>(e);
			}
		}
	};

	lua["Destory"] = sol::overload(destory1, destory2);

	lua["ApplyCueToTarget"] = [&registry](entt::entity e, std::string_view cue)
	{
		if (registry.all_of<View>(e))
		{
			auto& view = registry.get<View>(e);
			view.value->ApplyCue(cue);
		}
	};

	auto get_transform1 = [&registry](Remote target)
	{
		Transform* transform = nullptr;
		entt::entity e = registry.get(target);
		if (e != entt::null)
		{
			transform = registry.try_get<Transform>(e);
		}

		return transform;
	};

	auto get_transform2 = [&registry](entt::entity e)
	{
		return registry.try_get<Transform>(e);
	};

	auto get_transform3 = [&registry](const Local& target)
	{
		Transform* transform = nullptr;
		entt::entity e = registry.get(target);
		if (e != entt::null)
		{
			transform = registry.try_get<Transform>(e);
		}

		return transform;
	};

	lua["GetTransform"] = sol::overload(get_transform1,get_transform2,get_transform3);

	lua["CreateView"] = [&registry](Remote source,std::string_view asset, const Transform& transform)
	{
		entt::entity source_e = registry.get(source);
		if (source_e != entt::null)
		{
			auto e = registry.create_predict(source_e);
			registry.emplace<ViewAsset>(e, asset);
			registry.emplace<Transform>(e, transform);

			return e;
		}

		return static_cast<entt::entity>(entt::null);
	};

	lua["CreateMovementView"] = [&registry](
		Remote source,
		std::string_view asset,
		const Transform& transform,
		const vec3& forward,
		const vec3& velocity,
		const vec3& angular_velocity)
	{
		entt::entity source_e = registry.get(source);
		if (source_e != entt::null)
		{
			auto e = registry.create_predict(source_e);
			registry.emplace<ViewAsset>(e, asset);
			registry.emplace<Transform>(e, transform);
			registry.emplace<TransformState>(e);
			registry.emplace<MovementForward>(e, forward);
			registry.emplace<Movement>(e, velocity, angular_velocity);

			return e;
		}

		return static_cast<entt::entity>(entt::null);
	};

	lua["EmplaceMovementForward"] = [&registry](entt::entity e, const vec3& value)
	{
		registry.emplace<MovementForward>(e, value);
	};

	lua["EmplaceMovement"] = [&registry](entt::entity e,const vec3& velocity,const vec3& angular_velocity)
	{
		registry.emplace<Movement>(e,velocity,angular_velocity);
	};

	lua["EmplaceTransformState"] = [&registry](entt::entity e)
	{
		registry.emplace<TransformState>(e);
	};

	lua["EmplaceColliderInfo"] = [&registry](entt::entity e, const Collider& collider)
	{
		registry.emplace<Collider>(e, collider);
	};

	lua["CommitGameplayAbilityCost"] = [&registry](entt::entity ability)
	{
		gameplay_ability_utility::CommitGameplayAbilityCost(registry, ability);
	};

	lua["CommitGameplayAbilityCooldown"] = [&registry](entt::entity ability)
	{
		gameplay_ability_utility::CommitGameplayAbilityCooldown(registry, ability);
	};

	auto ApplyGameplayEffectToTarget1 = [&registry](entt::entity source, std::string_view effect_name, uint32_t level, entt::entity target)
	{
		GameplayEffectUtility::ApplyGameEffectToTarget(registry, source, effect_name, level, target);
	};

	auto ApplyGameplayEffectToTarget2 = [&registry](entt::entity source, const GameplayEffectSpec& spec)
	{
		GameplayEffectUtility::ApplyGameEffectToTarget(registry, source, spec);
	};

	lua["ApplyGameplayEffectToTarget"] = sol::overload(ApplyGameplayEffectToTarget1, ApplyGameplayEffectToTarget2);

	lua["MakeGameplayEffectSpec"] = [&registry](entt::entity source, std::string_view effect, uint32_t level, entt::entity target)
	{
		return GameplayEffectUtility::MakeGameplayEffectSpec(registry, source, effect, level, target);
	};

	auto GetEntity1 = [&registry](Remote id)
	{
		return registry.get(id);
	};

	auto GetEntity2 = [&registry](const Local& local)
	{
		return registry.get(local);
	};

	lua["GetEntity"] = sol::overload(GetEntity1, GetEntity2);

	lua["GetGameplayAbilitySpec"] = [&registry](entt::entity e)
	{
		return registry.get<GameplayAbilitySpec>(e);
	};

	lua["GetLocal"] = [&registry](entt::entity e)
	{
		return registry.get<Local>(e);
	};

	lua["GetGameplayTagCountContainer"] = [&registry](entt::entity e)
	{
		return registry.try_get<GameplayTagCountContainer>(e);
	};
}