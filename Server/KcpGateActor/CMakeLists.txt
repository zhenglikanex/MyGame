add_library(KcpGateActor SHARED KcpGateActor.hpp KcpGateActor.cpp)

target_link_libraries(KcpGateActor
    ActorNet
)
SET_PROPERTY(TARGET KcpGateActor PROPERTY FOLDER "Server")