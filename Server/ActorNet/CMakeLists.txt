add_library(ActorNet
    ActorLoader.h
    ActorLoader.cpp
    Config.hpp
    ActorNet.h
    ActorNet.cpp
    Actor.h
    Actor.cpp
    ActorMessage.h
    MessageCore.h
    MessageCore.cpp
    Common/Singleton.h
    Common/Singleton.cpp
    Common/typedef.h
    Common/Util.h
    NetworkActor.hpp
    NetworkActor.cpp
    NetworkComponent.hpp
    NetworkComponent.cpp
    main.cpp
)

target_link_libraries(ActorNet
)

SET_PROPERTY(TARGET ActorNet PROPERTY FOLDER "Server")
