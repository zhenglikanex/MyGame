#include "ServerGameHelper.hpp"

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
#include "Framework/Game/System/DebugSystem.hpp"
#include "Framework/Game/System/PhysicsSystem.hpp"
#include "Framework/Game/System/KinematicCharacterMovementSystem.hpp"
#include "Framework/Game/System/DestorySystem.hpp"
#include "Framework/Game/System/GAS/ApplyGameplayEffectSystem.hpp"
#include "Framework/Game/System/GAS/GameplayEffectSystem.hpp"
#include "Framework/Game/System/GAS/GameplayAbilityCommandSystem.hpp"
#include "Framework/Game/System/GAS/GameplayAbilitySystem.hpp"
#include "Framework/Game/system/GAS/GameplayAttributeSystem.hpp"
#include "Framework/Game/System/GAS/GameplayEventSystem.hpp"
#include "Framework/Game/System/GAS/GameplayStateSystem.hpp"
#include "Framework/Game/System/GAS/GameplayAbilitySystem.hpp"
#include "Framework/Game/System/GAS/GameplayWaitTaskSystem.hpp"
#include "Framework/Game/System/GAS/GameplayPlayAnimAndWaitTaskSystem.hpp"
#include "Framework/Game/System/GAS/GameplayInputTaskSystem.hpp"
#include "Framework/Game/System/GAS/GameplayHitTestTaskSystem.hpp"
#include "Framework/Game/System/GAS/DestroyModifierAttributeSystem.hpp"
#include "Framework/Game/System/ComputeAnimBoneChildTransform.hpp"

#include "Server/BattleActor/System/ServerSyncSystem.hpp"

ServerGameHelper::~ServerGameHelper()
{

}

bool ServerGameHelper::IsAuthority() const
{
	return true;
}

std::vector<std::unique_ptr<System>> ServerGameHelper::CreateSystems(Registry* registry) const
{
	std::vector<std::unique_ptr<System>> systems;
	systems.emplace_back(std::make_unique<CreateActorSystem>(*registry));
	systems.emplace_back(std::make_unique<CreateAnimationSystem>(*registry));
	systems.emplace_back(std::make_unique<CreatePhysicsActorSystem>(*registry));
	systems.emplace_back(std::make_unique<CommandSystem>(*registry));
	systems.emplace_back(std::make_unique<MoveCommandSystem>(*registry));
	systems.emplace_back(std::make_unique<GameplayAbilityCommandSystem>(*registry));
	systems.emplace_back(std::make_unique<ActorSystem>(*registry));
	systems.emplace_back(std::make_unique<GameplayAbilitySystem>(*registry));
	systems.emplace_back(std::make_unique<GameplayWaitTaskSystem>(*registry));
	systems.emplace_back(std::make_unique<GameplayPlayAnimAndWaitTaskSystem>(*registry));
	systems.emplace_back(std::make_unique<GameplayPlayInputSystem>(*registry));
	systems.emplace_back(std::make_unique<GameplayHitTestTaskSystem>(*registry));
	systems.emplace_back(std::make_unique<GameplayEventSystem>(*registry));
	systems.emplace_back(std::make_unique<ApplyGameplayEffectSystem>(*registry));
	systems.emplace_back(std::make_unique<GameplayEffectSystem>(*registry));
	systems.emplace_back(std::make_unique<GameplayAttributeSystem>(*registry));
	systems.emplace_back(std::make_unique<GameplayStateSystem>(*registry));
	systems.emplace_back(std::make_unique<RootMotionSystem>(*registry));
	systems.emplace_back(std::make_unique<TransformStateSystem>(*registry));
	systems.emplace_back(std::make_unique<AnimationSystem>(*registry));
	systems.emplace_back(std::make_unique<MovementSystem>(*registry));
	systems.emplace_back(std::make_unique<KinematicCharacterMovementSystem>(*registry));
	systems.emplace_back(std::make_unique<ComputeAnimBoneChildTransformSystem>(*registry));
	systems.emplace_back(std::make_unique<PhysicsSystem>(*registry));
	systems.emplace_back(std::make_unique<ServerSyncSystem>(*registry));
	systems.emplace_back(std::make_unique<DestorySystem>(*registry));
	systems.emplace_back(std::make_unique<DestroyModifierAttributeSystem>(*registry));

	return systems;
}

bool ServerGameHelper::IsLocalPlayer(uint32_t id) const
{
	return true;
}

bool ServerGameHelper::IsRemotePlayer(uint32_t id) const
{
	return false;
}
