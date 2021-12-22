#include <unordered_map>
#include <cstdint>

#include "Framework/Game/Component/UniqueId.hpp"
#include "Framework/Game/Component/Transform.hpp"
#include "Framework/Game/Component/AnimationClip.hpp"
#include "Framework/Game/Component/GAS/GameplayAttributeSet.hpp"
#include "Framework/Game/Component/GAS/GameplayTags.hpp"
#include "Framework/Game/Component/GAS/GameplayEffectSpec.hpp"
#include "Framework/Game/Component/GAS/GameplayAbilitySpec.hpp"

#include "Kanex.hpp"

struct FrameData
{
	uint32_t frame;
	std::unordered_map<UniqueId, Transform> transforms;
	std::unordered_map<UniqueId, AnimationClip> animation_clips;
	std::unordered_map<UniqueId, GameplayAttributeSet> attribute_sets;
	std::unordered_map<UniqueId, GameplayTags> tags;
	std::unordered_map<UniqueId, GameplayAbilitySpec> ability_sepcs;
	std::unordered_map<UniqueId, GameplayEffectSpec> effect_specs;
};

NON_MEMEBER_BAR(FrameData,obj.frame,obj.transforms,obj.animation_clips,obj.attribute_sets,obj.tags,obj.ability_sepcs,obj.effect_specs);