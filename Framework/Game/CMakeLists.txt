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
UIService.hpp
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
RandomEngine.hpp
LuaBridge.hpp


GAS/GameplayEffect.hpp
GAS/GameplayTag.hpp
GAS/GameplayEffectManager.hpp
GAS/GameplayAttributeData.hpp
GAS/GameplayAbility.hpp
GAS/GameplayAbilityManager.hpp

Utility/ActorStateUtility.hpp
Utility/AnimationUtility.hpp
Utility/CollisionUtility.hpp
Utility/ActorUtility.hpp
Utility/PhysicsUtility.hpp
Utility/GameplayAttributeUtility.hpp
Utility/GameplayEffectUtility.hpp
Utility/GameplayAbilityUtility.hpp
Utility/GameplayEventUtility.hpp
Utility/AnimEventUtility.hpp
Utility/RollbackUtility.hpp

System/AnimationSystem.hpp
System/CollisionSystem.hpp
System/CreateViewSystem.hpp
System/MovementSystem.hpp
System/UpdateViewSystem.hpp
System/ActorControlSystem.hpp
System/CreateAnimationSystem.hpp
System/RootMotionSystem.hpp
System/SkillStateSystem.hpp
System/CreateActorSystem.hpp
System/CreateSkillGraphSystem.hpp
System/DebugSystem.hpp
System/SkillSystem.hpp
System/UpdateColliderTransformSystem.hpp
System/NetworkReceiveSystem.hpp
System/NetworkSendSystem.hpp
System/TransformStateSystem.hpp
System/PhysicsSystem.hpp
System/KinematicCharacterMovementSystem.hpp
System/CreatePhysicsActorSystem.hpp
System/DelayTaskSystem.hpp
System/DestorySystem.hpp
System/UpdatePlayerAttributeUISystem.hpp
System/SyncInstanceStateSystem.hpp
System/ComputeAnimBoneChildTransform.hpp
System/GAS/ApplyGameplayEffectSystem.hpp
System/GAS/GameplayEffectSystem.hpp
System/GAS/GameplayAttributeSystem.hpp
System/GAS/GameplayAbilitySystem.hpp
System/GAS/TryGameplayAbilitySystem.hpp
System/GAS/GameplayEventSystem.hpp
System/GAS/GameplayStateSystem.hpp
System/GAS/GameplayWaitTaskSystem.hpp
System/GAS/GameplayPlayAnimAndWaitTaskSystem.hpp
System/GAS/GameplayInputTaskSystem.hpp
System/GAS/GameplayHitTestTaskSystem.hpp

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
Component/ColliderInfo.hpp
Component/Matrix4x4.hpp
Component/PhysicsWorld.hpp
Component/PhysicsWorld.cpp
Component/Character.hpp
Component/PhysicsActor.hpp
Component/DelayTask.hpp
Component/Destory.hpp
Component/UniqueId.hpp
Component/EntityByUniqueIdMap.hpp
Component/Local.hpp
Component/SyncInstanceState.hpp
Component/SyncManager.hpp
Component/CollisionReceiver.hpp
Component/ParentAnimBone.hpp
Component/GAS/ApplyGameplayEffect.hpp
Component/GAS/GameplayEffectSpec.hpp
Component/GAS/GameplayTags.hpp
Component/GAS/GameplayAttributeSet.hpp
Component/GAS/ModifierAttribute.hpp
Component/GAS/GameplayAbilitySpec.hpp
Component/GAS/GameplayAbilityScript.hpp
Component/GAS/TryGameplayAbility.hpp
Component/GAS/GameplayEvent.hpp
Component/GAS/GameplayState.hpp
Component/GAS/GameplayWaitTask.hpp
Component/GAS/GameplayPlayAnimAndWaitTask.hpp
Component/GAS/GameplayInputTask.hpp
Component/GAS/GameplayHitTestTask.hpp

ConfigManager.hpp
ConfigManager.cpp

Data/Config.hpp
Data/Config.cpp
Data/AnimationConfig.hpp
Data/ActorConfig.hpp
Data/ColliderConfig.hpp
Data/SkillGraph.hpp
Data/SkillGraph.cpp
Data/SceneConfig.hpp
)

foreach(_file ${file_list})
    make_source_group(${_file})
endforeach()

add_library(Game
    ${file_list}
)

add_library(Natvis INTERFACE)

if(MSVC)
    target_sources(
        Natvis
        INTERFACE
            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/3rdparty/src/entt-3.9.0/natvis/entt/config.natvis>
            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/3rdparty/src/entt-3.9.0/natvis/entt/container.natvis>
            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/3rdparty/src/entt-3.9.0/natvis/entt/core.natvis>
            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/3rdparty/src/entt-3.9.0/natvis/entt/entity.natvis>
            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/3rdparty/src/entt-3.9.0/natvis/entt/locator.natvis>
            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/3rdparty/src/entt-3.9.0/natvis/entt/meta.natvis>
            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/3rdparty/src/entt-3.9.0/natvis/entt/platform.natvis>
            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/3rdparty/src/entt-3.9.0/natvis/entt/poly.natvis>
            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/3rdparty/src/entt-3.9.0/natvis/entt/process.natvis>
            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/3rdparty/src/entt-3.9.0/natvis/entt/resource.natvis>
            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/3rdparty/src/entt-3.9.0/natvis/entt/signal.natvis>
    )
endif()

target_link_libraries(Game
    Natvis
    lua
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