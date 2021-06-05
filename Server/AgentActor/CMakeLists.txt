add_library(AgentActor SHARED AgentActor.hpp AgentActor.cpp) 
target_link_libraries(AgentActor
    ActorNet
    Proto
)
SET_PROPERTY(TARGET AgentActor PROPERTY FOLDER "Server")