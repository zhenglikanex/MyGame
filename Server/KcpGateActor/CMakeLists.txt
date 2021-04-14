add_library(KcpGateActor SHARED 
    KcpGateActor.hpp 
    KcpGateActor.cpp
    KcpConnection.hpp
    KcpConnection.cpp
    Kcp/ikcp.h
    Kcp/ikcp.c    
    )

target_link_libraries(KcpGateActor
    ActorNet
)
SET_PROPERTY(TARGET KcpGateActor PROPERTY FOLDER "Server")