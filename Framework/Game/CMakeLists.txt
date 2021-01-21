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

Utility/ActionStateUtility.hpp

System/AnimationSystem.hpp
System/CollisionSystem.hpp
System/CreateViewSystem.hpp
System/CommandProcessSystem.hpp
System/MovementSystem.hpp
System/UpdateViewSystem.hpp
System/SyncSystem.hpp
System/ActionStateMachineSystem.hpp

Component/GameMode.hpp
Component/GameState.hpp
Component/Command.hpp
Component/Player.hpp
Component/View.hpp
Component/Transform.hpp
Component/Asset.hpp
Component/Movement.hpp
Component/MainPlayer.hpp
Component/State.hpp
Component/ActionState.hpp
Component/Animation.hpp

ConfigManager.hpp
ConfigManager.cpp

Data/Config.hpp
Data/Config.cpp
Data/RootMotionConfig.hpp
Data/RootMotionConfig.cpp
)

foreach(_file ${file_list})
    make_source_group(${_file})
endforeach()

add_library(Game
    ${file_list}
)