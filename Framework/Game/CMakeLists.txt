set(file_list 
GameConfig.hpp
Game.hpp
Game.cpp
System.hpp
Locator.hpp
FixedPoint.hpp
Service.hpp
LogService.hpp
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

Utility/ActorStateUtility.hpp
Utility/AnimationUtility.hpp

System/AnimationSystem.hpp
System/CollisionSystem.hpp
System/CreateViewSystem.hpp
System/MovementSystem.hpp
System/UpdateViewSystem.hpp
System/ActorStateSystem.hpp
System/CreateAnimationSystem.hpp
System/RootMotionSystem.hpp
System/SkillStateSystem.hpp

Component/GameMode.hpp
Component/GameState.hpp
Component/Command.hpp
Component/Player.hpp
Component/View.hpp
Component/Transform.hpp
Component/ViewAsset.hpp
Component/AnimationAsset.hpp
Component/Movement.hpp
Component/ActorState.hpp
Component/Animation.hpp
Component/AnimationClip.hpp
Component/SkillState.hpp

ConfigManager.hpp
ConfigManager.cpp

Data/Config.hpp
Data/Config.cpp
Data/RootMotionConfig.hpp
Data/RootMotionConfig.cpp
Data/AnimationConfig.hpp
)

foreach(_file ${file_list})
    make_source_group(${_file})
endforeach()

add_library(Game
    ${file_list}
)