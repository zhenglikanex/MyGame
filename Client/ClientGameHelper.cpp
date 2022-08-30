#include "ClientGameHelper.hpp"

#include "Framework/Game/Game.hpp"

#include "Framework/Game/System/CommandSystem.hpp"
#include "Framework/Game/System/MoveCommandSystem.hpp"
#include "Framework/Game/System/CreateActorSystem.hpp"
#include "Framework/Game/System/CreateViewSystem.hpp"
#include "Framework/Game/System/CreateAnimationSystem.hpp"
#include "Framework/Game/System/CreatePhysicsActorSystem.hpp"
#include "Framework/Game/System/ActorSystem.hpp"
#include "Framework/Game/System/MovementSystem.hpp"
#include "Framework/Game/System/TransformStateSystem.hpp"
#include "Framework/Game/System/UpdateViewSystem.hpp"
#include "Framework/Game/System/AnimationSystem.hpp"
#include "Framework/Game/System/RootMotionSystem.hpp"
#include "Framework/Game/System/DebugSystem.hpp"
#include "Framework/Game/System/PhysicsSystem.hpp"
#include "Framework/Game/System/KinematicCharacterMovementSystem.hpp"
#include "Framework/Game/System/DestorySystem.hpp"
#include "Framework/Game/System/GAS/ApplyGameplayEffectSystem.hpp"
#include "Framework/Game/System/GAS/GameplayEffectSystem.hpp"
#include "Framework/Game/System/GAS/GameplayAbilityCommandSystem.hpp"
#include "Framework/Game/System/GAS/GameplayAbilitySystem.hpp"
#include "Framework/Game/System/GAS/GameplayAttributeSystem.hpp"
#include "Framework/Game/System/GAS/GameplayEventSystem.hpp"
#include "Framework/Game/System/GAS/GameplayStateSystem.hpp"
#include "Framework/Game/System/GAS/GameplayWaitTaskSystem.hpp"
#include "Framework/Game/System/GAS/GameplayPlayAnimAndWaitTaskSystem.hpp"
#include "Framework/Game/System/GAS/GameplayInputTaskSystem.hpp"
#include "Framework/Game/System/GAS/GameplayHitTestTaskSystem.hpp"
#include "Framework/Game/System/GAS/DestroyModifierAttributeSystem.hpp"
#include "Framework/Game/System/GAS/GameplayAbilityTimelineTaskSystem.hpp"
#include "Framework/Game/System/GAS/GameplayAnimWaitTaskSystem.hpp"
#include "Framework/Game/System/UpdatePlayerUISystem.hpp"
#include "Framework/Game/System/LocalEntityStateSystem.hpp"
#include "Framework/Game/System/ComputeAnimBoneChildTransform.hpp"
#include "Framework/Game/System/FlyingTextSystem.hpp"


#include "Client/System/InputSystem.hpp"
#include "Client/System/ClientSyncSystem.hpp"


ClientGameHelper::ClientGameHelper(uint32_t local_id)
	: GameHelper()
	, local_id_(local_id)
{

}

GameMode ClientGameHelper::GetGameMode() const
{
	return GameMode::kSimulation;
}

void ClientGameHelper::CreateSystems(Game* game) const
{
	game->AddSystem <
		InputSystem,
		CommandSystem,
		MoveCommandSystem,
		GameplayAbilityCommandSystem,
		CreateActorSystem,
		CreateViewSystem,
		CreateAnimationSystem,
		CreatePhysicsActorSystem,
		ActorSystem,
		GameplayAbilitySystem,
		GameplayAbilityTimelineTaskSystem,
		RootMotionSystem,
		AnimationSystem,
		GameplayWaitTaskSystem,
		GameplayPlayAnimAndWaitTaskSystem,
		GameplayPlayInputSystem,
		GameplayHitTestTaskSystem,
		GameplayAnimWaitTaskSystem,
		GameplayEventSystem,
		ApplyGameplayEffectSystem,
		GameplayEffectSystem,
		GameplayAttributeSystem,
		MovementSystem,
		KinematicCharacterMovementSystem,
		ComputeAnimBoneChildTransformSystem,
		PhysicsSystem,
		ClientSyncSystem,
		TransformStateSystem,
		GameplayStateSystem,
		LocalEntityStateSystem,
		DestorySystem,
		UpdatePlayerUISystem,
		UpdateViewSystem,
		FlyingTextSystem,
		DestroyModifierAttributeSystem,
#if DEBUG
		DebugSystem
#endif
	> ();
}

uint64_t ClientGameHelper::GameFrameTime() const
{
	// todo:可以根据延迟动态调整
	return GameConfig::kFrameTimeNanoseconds;
}
