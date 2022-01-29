#include "ClientGameHelper.hpp"

#include "Framework/Game/Registry.hpp"

#include "Framework/Game/System/CreateActorSystem.hpp"
#include "Framework/Game/System/CreateViewSystem.hpp"
#include "Framework/Game/System/CreateAnimationSystem.hpp"
#include "Framework/Game/System/CreatePhysicsActorSystem.hpp"
#include "Framework/Game/System/ActorControlSystem.hpp"
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
#include "Framework/Game/System/GAS/TryGameplayAbilitySystem.hpp"
#include "Framework/Game/System/GAS/GameplayAbilitySystem.hpp"
#include "Framework/Game/system/GAS/GameplayAttributeSystem.hpp"
#include "Framework/Game/System/GAS/GameplayEventSystem.hpp"
#include "Framework/Game/System/GAS/GameplayStateSystem.hpp"
#include "Framework/Game/System/GAS/GameplayAbilitySystem.hpp"
#include "Framework/Game/System/GAS/GameplayWaitTaskSystem.hpp"
#include "Framework/Game/System/GAS/GameplayPlayAnimAndWaitTaskSystem.hpp"
#include "Framework/Game/System/GAS/GameplayInputTaskSystem.hpp"
#include "Framework/Game/System/GAS/GameplayHitTestTaskSystem.hpp"
#include "Framework/Game/System/UpdatePlayerUISystem.hpp"
#include "Framework/Game/System/SyncInstanceStateSystem.hpp"
#include "Framework/Game/System/ComputeAnimBoneChildTransform.hpp"
#include "Client/System/ClientSyncSystem.hpp"

ClientGameHelper::ClientGameHelper(uint32_t local_id)
	: GameHelper()
	, local_id_(local_id)
{

}

ClientGameHelper::~ClientGameHelper()
{

}

bool ClientGameHelper::IsAuthority() const
{
	return false;
}

std::vector<std::unique_ptr<System>> ClientGameHelper::CreateSystems(Registry* registry) const
{
	std::vector<std::unique_ptr<System>> systems;

	systems.emplace_back(std::make_unique<CreateActorSystem>(*registry));
	systems.emplace_back(std::make_unique<CreateViewSystem>(*registry));
	systems.emplace_back(std::make_unique<CreateAnimationSystem>(*registry));
	systems.emplace_back(std::make_unique<CreatePhysicsActorSystem>(*registry));
	systems.emplace_back(std::make_unique<ActorControlSystem>(*registry));
	systems.emplace_back(std::make_unique<RootMotionSystem>(*registry));
	systems.emplace_back(std::make_unique<MovementSystem>(*registry));
	systems.emplace_back(std::make_unique<KinematicCharacterMovementSystem>(*registry));
	systems.emplace_back(std::make_unique<AnimationSystem>(*registry));
	systems.emplace_back(std::make_unique<TryGameplayAbilitySystem>(*registry));
	systems.emplace_back(std::make_unique<GameplayAbilitySystem>(*registry));
	systems.emplace_back(std::make_unique<GameplayWaitTaskSystem>(*registry));
	systems.emplace_back(std::make_unique<GameplayPlayAnimAndWaitTaskSystem>(*registry));
	systems.emplace_back(std::make_unique<GameplayPlayInputSystem>(*registry));
	systems.emplace_back(std::make_unique<GameplayHitTestTaskSystem>(*registry));
	systems.emplace_back(std::make_unique<GameplayEventSystem>(*registry));
	systems.emplace_back(std::make_unique<ApplyGameplayEffectSystem>(*registry));
	systems.emplace_back(std::make_unique<GameplayAttributeSystem>(*registry));
	systems.emplace_back(std::make_unique<ComputeAnimBoneChildTransformSystem>(*registry));
	systems.emplace_back(std::make_unique<PhysicsSystem>(*registry));
	systems.emplace_back(std::make_unique<DestorySystem>(*registry));
	systems.emplace_back(std::make_unique<ClientSyncSystem>(*registry));
	systems.emplace_back(std::make_unique<TransformStateSystem>(*registry));
	systems.emplace_back(std::make_unique<GameplayStateSystem>(*registry));
	systems.emplace_back(std::make_unique<SyncInstanceStateSystem>(*registry));
	systems.emplace_back(std::make_unique<UpdatePlayerUISystem>(*registry));
	systems.emplace_back(std::make_unique<UpdateViewSystem>(*registry));
#if DEBUG
	systems.emplace_back(std::make_unique<DebugSystem>(*registry));
#endif

	return systems;
}

bool ClientGameHelper::IsLocalPlayer(uint32_t id) const
{
	return id == local_id_;
}

bool ClientGameHelper::IsRemotePlayer(uint32_t id) const
{
	return id != local_id_;
}