set(file_list 
GameConfig.hpp
GameMode.hpp
Game.hpp
Game.cpp
System.hpp
Locator.hpp
FixedPoint.hpp
Service.hpp
DebugService.hpp
InputService.hpp
FileService.hpp
FileService.cpp
ViewService.hpp
IViewImpl.hpp
NetworkService.hpp
Json.hpp
Math.hpp
Fmt.hpp
PlayerInfo.hpp
Hash.hpp
Collision.hpp
Collision.cpp
Geometry.hpp
GameHelper.hpp
Proto.hpp

Utility/ActorStateUtility.hpp
Utility/AnimationUtility.hpp
Utility/CollisionUtility.hpp
Utility/ActorUtility.hpp
Utility/PhysicsUtility.hpp

System/AnimationSystem.hpp
System/CollisionSystem.hpp
System/CreateViewSystem.hpp
System/MovementSystem.hpp
System/UpdateViewSystem.hpp
System/ActorStateSystem.hpp
System/CreateAnimationSystem.hpp
System/RootMotionSystem.hpp
System/SkillStateSystem.hpp
System/CreateActorSystem.hpp
System/CreateSkillGraphSystem.hpp
System/DebugSystem.hpp
System/SkillSystem.hpp
System/ModifyHealthSystem.hpp
System/HealthSystem.hpp
System/UpdateColliderTransformSystem.hpp
System/NetworkReceiveSystem.hpp
System/NetworkSendSystem.hpp
System/TransformStateSystem.hpp
System/PhysicsSystem.hpp

Component/Remote.hpp
Component/Local.hpp
Component/GameState.hpp
Component/Command.hpp
Component/Player.hpp
Component/View.hpp
Component/Transform.hpp
Component/TransformState.hpp
Component/ViewAsset.hpp
Component/AnimationAsset.hpp
Component/Movement.hpp
Component/ActorState.hpp
Component/Animation.hpp
Component/AnimationClip.hpp
Component/SkillState.hpp
Component/Collider.hpp
Component/ActorAsset.hpp
Component/Weapon.hpp
Component/SkillGraphAsset.hpp
Component/Skill.hpp
Component/Contact.hpp
Component/Health.hpp
Component/Attributes.hpp
Component/ColliderInfo.hpp
Component/Matrix4x4.hpp
Component/PhysicsWorld.hpp
Component/PhysicsWorld.cpp

ConfigManager.hpp
ConfigManager.cpp

Data/Config.hpp
Data/Config.cpp
Data/AnimationConfig.hpp
Data/ActorConfig.hpp
Data/ColliderConfig.hpp
Data/SkillGraph.hpp
Data/SkillGraph.cpp
)

foreach(_file ${file_list})
    make_source_group(${_file})
endforeach()

add_library(Game
    ${file_list}
)

target_link_libraries(Game
    debug ${PHYSX_LIBRARY_DEBU0G}
    debug ${PHYSX_COMMON_LIBRARY_DEBU0G}
    debug ${PX_FOUNDATION_LIBARY_DEBUG}
    debug ${PHYSX_COOKING_LIBRARY_DEBUG}
    debug ${PX_PVD_LIBARY_DEBUG}
    debug ${PHYSX_EXTENSIONS_LIBRARY_DEBU0G}

    optimized ${PHYSX_LIBRARY}
    optimized ${PHYSX_COMMON_LIBRARY}
    optimized ${PX_FOUNDATION_LIBARY}
    optimized ${PHYSX_COOKING_LIBRARY}
    optimized ${PX_PVD_LIBARY}
    optimized ${PHYSX_EXTENSIONS_LIBRARY}
)