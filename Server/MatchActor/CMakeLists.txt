add_library(MatchActor SHARED MatchActor.hpp MatchActor.cpp) 
target_link_libraries(MatchActor
    ActorNet
    Proto
)
SET_PROPERTY(TARGET MatchActor PROPERTY FOLDER "Server")