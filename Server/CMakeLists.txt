include_directories("${CMAKE_CURRENT_SOURCE_DIR}/ActorNet")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/ActorNet/Common")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/ActorNet/Network")


add_subdirectory(ActorNet)
add_subdirectory(StartActor)
add_subdirectory(LogActorActor)
add_subdirectory(KcpGateActor)
add_subdirectory(ServerApp)
add_subdirectory(AgentActor)
add_subdirectory(MatchActor)
