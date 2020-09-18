add_executable(ActorNet
    ActorLoader.h
    ActorLoader.cpp
    ActorNet.h
    ActorNet.cpp
    IActor.h
    MessageCore.h
    MessageCore.cpp
    Common/Singleton.h
    Common/Singleton.cpp
    Common/typedef.h
    Common/Util.h
    Network/Network.h
    Network/Network.cpp
    Network/ISessionHandler.h
    Network/Message.h 
    Network/Message.cpp
    Network/Session.h 
    Network/Session.cpp
    Network/Timer.h
    Network/Timer.cpp
    main.cpp
)

add_definitions(-D _WIN32_WINNT)
add_definitions(-D ASIO_STANDALONE)


SET_PROPERTY(TARGET ActorNet PROPERTY FOLDER "Server")
