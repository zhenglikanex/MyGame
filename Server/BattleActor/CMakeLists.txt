add_library(BattleActor SHARED 
    BattleActor.hpp
    BattleActor.cpp
    ServerDebugService.hpp
    ServerDebugService.cpp
    ServerInputService.hpp
    ServerInputService.cpp
    ServerFileService.hpp
    ServerFileService.cpp
    ServerInputService.hpp
    ServerInputService.cpp
    ServerViewService.hpp
    ServerViewService.cpp
    ServerNetworkService.hpp
    ServerNetworkService.cpp
    ServerViewImpl.hpp
    ServerViewImpl.cpp
    ServerGameHelper.hpp
    ServerGameHelper.cpp
    System/ServerSyncSystem.hpp
    ) 

target_link_libraries(BattleActor
    ActorNet
    Proto
    Game
)
SET_PROPERTY(TARGET BattleActor PROPERTY FOLDER "Server")