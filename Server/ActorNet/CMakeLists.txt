add_executable(ActorNet
    ActorLoader.h
    ActorLoader.cpp
    ActorNet.h
    ActorNet.cpp
    IActor.h
    ActorMessage.h
    MessageCore.h
    MessageCore.cpp
    Common/Singleton.h
    Common/Singleton.cpp
    Common/typedef.h
    Common/Util.h
    Network/NetworkActor.hpp
    Network/NetworkActor.cpp
    Network/Connection.hpp
    Network/Connection.cpp
    Network/Network.h
    Network/Network.cpp
    Network/ISessionHandler.h
    Network/Message.h 
    Network/Message.cpp
    Network/Timer.h
    Network/Timer.cpp
    main.cpp
    ${PROTO_SRC}
)

target_link_libraries(ActorNet 
     debug ${PROTOBUF_LIBRARY_DEBUG} 
     debug ${PROTOBUF_LITE_LIBRARY_DEBUG}
     optimized ${PROTOBUF_LIBRARY}
     optimized ${PROTOBUF_LITE_LIBRARY} 
)

SET_PROPERTY(TARGET ActorNet PROPERTY FOLDER "Server")

#设置VS工作目录
set_target_properties(ActorNet PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/Asset)