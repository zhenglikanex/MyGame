#include "ServerGameHelper.hpp"

#include "Framework/Game/System/CreateActorSystem.hpp"
#include "Framework/Game/System/CreateAnimationSystem.hpp"
#include "Framework/Game/System/CreateSkillGraphSystem.hpp"
#include "Framework/Game/System/CreatePhysicsActorSystem.hpp"
#include "Framework/Game/System/ActorStateSystem.hpp"
#include "Framework/Game/System/MovementSystem.hpp"
#include "Framework/Game/System/TransformStateSystem.hpp"
#include "Framework/Game/System/AnimationSystem.hpp"
#include "Framework/Game/System/CollisionSystem.hpp"
#include "Framework/Game/System/RootMotionSystem.hpp"
#include "Framework/Game/System/SkillStateSystem.hpp"
#include "Framework/Game/System/DebugSystem.hpp"
#include "Framework/Game/System/SkillSystem.hpp"
#include "Framework/Game/System/UpdateColliderTransformSystem.hpp"
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

#include "Server/BattleActor/System/ServerSyncSystem.hpp"

ServerGameHelper::~ServerGameHelper()
{

}

bool ServerGameHelper::IsAuthority() const
{
	return true;
}

std::vector<std::unique_ptr<System>> ServerGameHelper::CreateSystems(entt::registry& registry) const
{
	std::vector<std::unique_ptr<System>> systems;
	systems.emplace_back(std::make_unique<CreateActorSystem>(registry));
	systems.emplace_back(std::make_unique<CreateAnimationSystem>(registry));
	systems.emplace_back(std::make_unique<CreateSkillGraphSystem>(registry));
	systems.emplace_back(std::make_unique<CreatePhysicsActorSystem>(registry));
	systems.emplace_back(std::make_unique<PhysicsSystem>(registry));
	systems.emplace_back(std::make_unique<ActorStateSystem>(registry));
	systems.emplace_back(std::make_unique<SkillStateSystem>(registry));
	systems.emplace_back(std::make_unique<RootMotionSystem>(registry));
	systems.emplace_back(std::make_unique<MovementSystem>(registry));
	systems.emplace_back(std::make_unique<KinematicCharacterMovementSystem>(registry));
	systems.emplace_back(std::make_unique<TransformStateSystem>(registry));
	systems.emplace_back(std::make_unique<SkillSystem>(registry));
	systems.emplace_back(std::make_unique<AnimationSystem>(registry));
	//systems.emplace_back(std::make_unique<UpdateColliderTransformSystem>(registry));
	//systems.emplace_back(std::make_unique<CollisionSystem>(registry));
	systems.emplace_back(std::make_unique<TryGameplayAbilitySystem>(registry));
	systems.emplace_back(std::make_unique<GameplayEventSystem>(registry));
	systems.emplace_back(std::make_unique<ApplyGameplayEffectSystem>(registry));
	systems.emplace_back(std::make_unique<GameplayAttributeSystem>(registry));
	systems.emplace_back(std::make_unique<GameplayStateSystem>(registry));
	systems.emplace_back(std::make_unique<ServerSyncSystem>(registry));
	systems.emplace_back(std::make_unique<DestorySystem>(registry));
	

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

bool ServerGameHelper::IsControllable(entt::registry& registry, entt::entity e) const
{
	return true;
}
