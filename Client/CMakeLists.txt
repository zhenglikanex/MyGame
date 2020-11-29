
add_library(MyGameClient SHARED
    main.cpp
    UnityBridge.hpp
    UnityBridge.cpp
    UnityViewImpl.hpp
    UnityViewImpl.cpp
    UnityViewService.hpp
    UnityViewService.cpp
    ${PROTO_SRC}
)

target_link_libraries(MyGameClient
    Game
    ${PROTOBUF_LIBRARY}
    ${PROTOBUF_LITE_LIBRARY}
)

