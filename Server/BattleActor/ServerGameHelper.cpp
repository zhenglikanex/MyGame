#include "ServerGameHelper.hpp"

#include "Framework/Game/Game.hpp"

#include "Framework/Game/System/RecvPlayerInputSystem.hpp"
#include "Framework/Game/System/CommandSystem.hpp"
#include "Framework/Game/System/MoveCommandSystem.hpp"
#include "Framework/Game/System/GAS/GameplayAbilityCommandSystem.hpp"
#include "Framework/Game/System/CreateActorSystem.hpp"
#include "Framework/Game/System/CreateAnimationSystem.hpp"
#include "Framework/Game/System/CreatePhysicsActorSystem.hpp"
#include "Framework/Game/System/ActorSystem.hpp"
#include "Framework/Game/System/MovementSystem.hpp"
#include "Framework/Game/System/TransformStateSystem.hpp"
#include "Framework/Game/System/AnimationSystem.hpp"
#include "Framework/Game/System/RootMotionSystem.hpp"
#include "Framework/Game/System/PhysicsSystem.hpp"
#include "Framework/Game/System/KinematicCharacterMovementSystem.hpp"
#include "Framework/Game/System/DestorySystem.hpp"
#include "Framework/Game/System/GAS/ApplyGameplayEffectSystem.hpp"
#include "Framework/Game/System/GAS/GameplayEffectSystem.hpp"
#include "Framework/Game/System/GAS/GameplayAbilitySystem.hpp"
#include "Framework/Game/system/GAS/GameplayAttributeSystem.hpp"
#include "Framework/Game/System/GAS/GameplayEventSystem.hpp"
#include "Framework/Game/System/GAS/GameplayStateSystem.hpp"
#include "Framework/Game/System/GAS/GameplayAbilityTimelineTaskSystem.hpp"
#include "Framework/Game/System/GAS/GameplayWaitTaskSystem.hpp"
#include "Framework/Game/System/GAS/GameplayPlayAnimAndWaitTaskSystem.hpp"
#include "Framework/Game/System/GAS/GameplayInputTaskSystem.hpp"
#include "Framework/Game/System/GAS/GameplayHitTestTaskSystem.hpp"
#include "Framework/Game/System/GAS/DestroyModifierAttributeSystem.hpp"
#include "Framework/Game/System/GAS/GameplayAnimWaitTaskSystem.hpp"
#include "Framework/Game/System/ComputeAnimBoneChildTransform.hpp"

#include "Server/BattleActor/System/ServerSyncSystem.hpp"

GameMode ServerGameHelper::GetGameMode() const
{
	return GameMode::kAuthority;
}

void ServerGameHelper::CreateSystems(Game* game) const
{
	game->AddSystem <
		RecvPlayerInputSystem,
		CommandSystem,
		MoveCommandSystem,
		GameplayAbilityCommandSystem,
		CreateActorSystem,
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
		//GameplayStateSystem,
		TransformStateSystem,
		MovementSystem,
		KinematicCharacterMovementSystem,
		ComputeAnimBoneChildTransformSystem,
		PhysicsSystem,
		//ServerSyncSystem,
		DestorySystem,
		DestroyModifierAttributeSystem
	> ();
}
