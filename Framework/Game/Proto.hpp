#include <unordered_map>
#include <cstdint>

#include "Framework/Game/Component/Transform.hpp"
#include "Framework/Game/Component/AnimationClip.hpp"

#include "Framework/Game/Component/GAS/GameplayAttributeSet.hpp"
#include "Framework/Game/Component/GAS/GameplayTags.hpp"
#include "Framework/Game/Component/GAS/GameplayEffectSpec.hpp"
#include "Framework/Game/Component/GAS/GameplayAbilitySpec.hpp"

#include "Kanex.hpp"

struct ActorData
{
	Transform transform;
	AnimationClip clip;
	GameplayAttributeSet attr_set;
	GameplayTagArray tags;
};

NON_MEMEBER_BAR(ActorData, obj.transform,obj.clip,obj.attr_set,obj.tags);

struct FrameData
{
	uint32_t frame;
	std::unordered_map<uint32_t, ActorData> actors;
};

NON_MEMEBER_BAR(FrameData, obj.frame, obj.actors);