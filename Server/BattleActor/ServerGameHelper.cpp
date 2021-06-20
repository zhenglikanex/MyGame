#include "ServerGameHelper.hpp"

#include "Framework/Game/System/CreateAnimationSystem.hpp"
#include "Framework/Game/System/CreateSkillGraphSystem.hpp"
#include "Framework/Game/System/ActorStateSystem.hpp"
#include "Framework/Game/System/MovementSystem.hpp"
#include "Framework/Game/System/AnimationSystem.hpp"
#include "Framework/Game/System/CollisionSystem.hpp"
#include "Framework/Game/System/RootMotionSystem.hpp"
#include "Framework/Game/System/SkillStateSystem.hpp"
#include "Framework/Game/System/DebugSystem.hpp"
#include "Framework/Game/System/HealthSystem.hpp"
#include "Framework/Game/System/ModifyHealthSystem.hpp"
#include "Framework/Game/System/SkillSystem.hpp"
#include "Framework/Game/System/UpdateColliderTransformSystem.hpp"

#include "Server/BattleActor/System/ServerSyncSystem.hpp"

ServerGameHelper::~ServerGameHelper()
{

}

std::vector<std::unique_ptr<System>> ServerGameHelper::CreateSystems(entt::registry& registry) const
{
	std::vector<std::unique_ptr<System>> systems;
	systems.emplace_back(std::make_unique<CreateAnimationSystem>(registry));
	systems.emplace_back(std::make_unique<CreateSkillGraphSystem>(registry));
	systems.emplace_back(std::make_unique<HealthSystem>(registry));
	systems.emplace_back(std::make_unique<ModifyHealthSystem>(registry));
	systems.emplace_back(std::make_unique<ActorStateSystem>(registry));
	systems.emplace_back(std::make_unique<SkillStateSystem>(registry));
	systems.emplace_back(std::make_unique<RootMotionSystem>(registry));
	systems.emplace_back(std::make_unique<MovementSystem>(registry));
	systems.emplace_back(std::make_unique<SkillSystem>(registry));
	systems.emplace_back(std::make_unique<AnimationSystem>(registry));
	systems.emplace_back(std::make_unique<UpdateColliderTransformSystem>(registry));
	systems.emplace_back(std::make_unique<CollisionSystem>(registry));
	systems.emplace_back(std::make_unique<ServerSyncSystem>(registry));

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
