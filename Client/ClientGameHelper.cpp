#include "ClientGameHelper.hpp"

#include "Framework/Game/System/CreateActorSystem.hpp"
#include "Framework/Game/System/CreateViewSystem.hpp"
#include "Framework/Game/System/CreateAnimationSystem.hpp"
#include "Framework/Game/System/CreateSkillGraphSystem.hpp"
#include "Framework/Game/System/ActorStateSystem.hpp"
#include "Framework/Game/System/MovementSystem.hpp"
#include "Framework/Game/System/TransformStateSystem.hpp"
#include "Framework/Game/System/UpdateViewSystem.hpp"
#include "Framework/Game/System/AnimationSystem.hpp"
#include "Framework/Game/System/CollisionSystem.hpp"
#include "Framework/Game/System/RootMotionSystem.hpp"
#include "Framework/Game/System/SkillStateSystem.hpp"
#include "Framework/Game/System/DebugSystem.hpp"
#include "Framework/Game/System/HealthSystem.hpp"
#include "Framework/Game/System/ModifyHealthSystem.hpp"
#include "Framework/Game/System/SkillSystem.hpp"
#include "Framework/Game/System/UpdateColliderTransformSystem.hpp"
#include "Framework/Game/System/PhysicsSystem.hpp"

#include "Client/System/ClientSyncSystem.hpp"

ClientGameHelper::ClientGameHelper(uint32_t local_id)
	: GameHelper()
	, local_id_(local_id)
{

}

ClientGameHelper::~ClientGameHelper()
{

}

std::vector<std::unique_ptr<System>> ClientGameHelper::CreateSystems(entt::registry& registry) const
{
	std::vector<std::unique_ptr<System>> systems;

	systems.emplace_back(std::make_unique<ClientSyncSystem>(registry));
	systems.emplace_back(std::make_unique<CreateActorSystem>(registry));
	systems.emplace_back(std::make_unique<CreateViewSystem>(registry));
	systems.emplace_back(std::make_unique<CreateAnimationSystem>(registry));
	systems.emplace_back(std::make_unique<CreateSkillGraphSystem>(registry));
	systems.emplace_back(std::make_unique<HealthSystem>(registry));
	systems.emplace_back(std::make_unique<ModifyHealthSystem>(registry));
	systems.emplace_back(std::make_unique<ActorStateSystem>(registry));
	systems.emplace_back(std::make_unique<SkillStateSystem>(registry));
	systems.emplace_back(std::make_unique<RootMotionSystem>(registry));
	systems.emplace_back(std::make_unique<MovementSystem>(registry));
	systems.emplace_back(std::make_unique<TransformStateSystem>(registry));
	systems.emplace_back(std::make_unique<SkillSystem>(registry));
	systems.emplace_back(std::make_unique<AnimationSystem>(registry));
	systems.emplace_back(std::make_unique<UpdateColliderTransformSystem>(registry));
	systems.emplace_back(std::make_unique<CollisionSystem>(registry));
	systems.emplace_back(std::make_unique<PhysicsSystem>(registry));
	systems.emplace_back(std::make_unique<UpdateViewSystem>(registry));

#if DEBUG
	systems.emplace_back(std::make_unique<DebugSystem>(registry));
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
