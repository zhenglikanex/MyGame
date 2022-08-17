add_library(StartActor SHARED
    StartActor.h
    StartActor.cpp  
)

target_link_libraries(StartActor
    ActorNet
)

SET_PROPERTY(TARGET StartActor PROPERTY FOLDER "Server")