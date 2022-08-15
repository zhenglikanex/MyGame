#include <unordered_map>
#include <cstdint>

#include "Framework/Game/Component/Remote.hpp"
#include "Framework/Game/Component/ActorAsset.hpp"
#include "Framework/Game/Component/Local.hpp"
#include "Framework/Game/Component/Player.hpp"
#include "Framework/Game/Component/Transform.hpp"
#include "Framework/Game/Component/AnimationClip.hpp"
#include "Framework/Game/Component/Movement.hpp"
#include "Framework/Game/Component/ViewAsset.hpp"
#include "Framework/Game/Component/Destroy.hpp"
#include "Framework/Game/Component/GAS/GameplayAttributeSet.hpp"
#include "Framework/Game/Component/GAS/GameplayEffectSpec.hpp"
#include "Framework/Game/Component/GAS/GameplayAbilitySpec.hpp"
#include "Framework/Game/Component/GAS/GameplayWaitTask.hpp"
#include "Framework/Game/Component/GAS/GameplayPlayAnimAndWaitTask.hpp"
#include "Framework/Game/Component/GAS/GameplayInputTask.hpp"
#include "Framework/Game/Component/GAS/GameplayHitTestTask.hpp"
#include "Framework/Game/Component/GAS/ModifierAttribute.hpp"

#include "Framework/Game/Component/TransformState.hpp"
#include "Framework/Game/Component/LocalEntityState.hpp"
#include "Framework/Game/Component/GAS/GameplayState.hpp"

#include "Kanex.hpp"

struct FrameData
{
	uint32_t frame;
	uint32_t local_entity_identifier;
	uint32_t local_entity_num;
};

NON_MEMEBER_BAR(FrameData, obj.frame, obj.local_entity_identifier, obj.local_entity_num);

template<class ... Components>
struct TypeList
{};

using LocalComponents = TypeList
<
	Remote,
	MovementForward,
	CharacterMovement,
	InfiniteModifierAttributeSet,
	GameplayWaitTask,
	GameplayPlayAnimAndWaitTask,
	GameplayInputTask,
	GameplayHitTestTask,
	IgnoreGravity
>;

using RemoteComponents = TypeList
<
	Player,
	ActorAsset,
	ViewAsset,
	Transform,
	AnimationClip,
	GameplayAttributeSet,
	GameplayTagCountContainer,
	GameplayAbilitySpec,
	GameplayEffectSpec,
	Destroy
>;

using SingleComponents = TypeList
<
	ModifierAttribute
>;