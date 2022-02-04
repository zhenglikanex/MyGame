#include <unordered_map>
#include <cstdint>

#include "Framework/Game/Component/UniqueId.hpp"
#include "Framework/Game/Component/Local.hpp"
#include "Framework/Game/Component/Player.hpp"
#include "Framework/Game/Component/Transform.hpp"
#include "Framework/Game/Component/AnimationClip.hpp"
#include "Framework/Game/Component/Movement.hpp"
#include "Framework/Game/Component/ViewAsset.hpp"
#include "Framework/Game/Component/Destory.hpp"
#include "Framework/Game/Component/GAS/GameplayAttributeSet.hpp"
#include "Framework/Game/Component/GAS/GameplayTags.hpp"
#include "Framework/Game/Component/GAS/GameplayEffectSpec.hpp"
#include "Framework/Game/Component/GAS/GameplayAbilitySpec.hpp"
#include "Framework/Game/Component/GAS/GameplayWaitTask.hpp"
#include "Framework/Game/Component/GAS/GameplayPlayAnimAndWaitTask.hpp"
#include "Framework/Game/Component/GAS/GameplayInputTask.hpp"
#include "Framework/Game/Component/GAS/GameplayHitTestTask.hpp"
#include "Framework/Game/Component/GAS/ModifierAttribute.hpp"

#include "Kanex.hpp"

struct FrameData
{
	uint32_t frame;
	uint32_t local_id;
	uint32_t local_num;
	std::unordered_map<Local, UniqueId> unique_ids;
	std::unordered_map<Local, MovementForward> movement_forwards;
	std::unordered_map<Local, CharacterMovement> character_movements;
	std::unordered_map<Local, InfiniteModifierAttributeSet> infinite_modifier_attribute_sets;
	std::unordered_map<Local, GameplayWaitTask> wait_tasks;
	std::unordered_map<Local, GameplayPlayAnimAndWaitTask> play_anim_and_wait_tasks;
	std::unordered_map<Local, GameplayInputTask> input_tasks;
	std::unordered_map<Local, GameplayHitTestTask> hit_test_tasks;
	std::unordered_map<UniqueId, Player> players;
	std::unordered_map<UniqueId, ActorAsset> actor_assets;
	std::unordered_map<UniqueId, ViewAsset> view_assets;
	std::unordered_map<UniqueId, Transform> transforms;
	std::unordered_map<UniqueId, AnimationClip> animation_clips;
	std::unordered_map<UniqueId, GameplayAttributeSet> attribute_sets;
	std::unordered_map<UniqueId, GameplayTags> tags;
	std::unordered_map<UniqueId, GameplayAbilitySpec> ability_specs;
	std::unordered_map<UniqueId, GameplayEffectSpec> effect_specs;
	std::unordered_set<UniqueId> destorys;
	std::vector<ModifierAttribute> modifier_attributes;
};

NON_MEMEBER_BAR(FrameData,
	obj.frame,
	obj.local_id,
	obj.local_num,
	obj.unique_ids,
	obj.movement_forwards,
	obj.players,
	obj.actor_assets,
	obj.view_assets,
	obj.transforms,
	obj.animation_clips,
	obj.character_movements,
	obj.attribute_sets,
	obj.tags,
	obj.ability_specs,
	obj.effect_specs,
	obj.wait_tasks,
	obj.destorys,
	obj.modifier_attributes);