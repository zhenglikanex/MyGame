add_library(KcpGateActor SHARED 
    KcpGateActor.hpp 
    KcpGateActor.cpp
    KcpConnection.hpp
    KcpConnection.cpp
    )

target_link_libraries(KcpGateActor
    ActorNet
)
SET_PROPERTY(TARGET KcpGateActor PROPERTY FOLDER "Server")