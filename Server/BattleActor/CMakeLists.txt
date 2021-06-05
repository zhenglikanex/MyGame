add_library(BattleActor SHARED BattleActor.hpp BattleActor.cpp) 
target_link_libraries(BattleActor
    ActorNet
    Proto
)
SET_PROPERTY(TARGET BattleActor PROPERTY FOLDER "Server")