
add_library(MyGameClient SHARED
    main.cpp
    UnityBridge.hpp
    UnityBridge.cpp
    UnityViewImpl.hpp
    UnityViewImpl.cpp
    UnityViewService.hpp
    UnityViewService.cpp
    UnityInputService.hpp
    UnityInputService.cpp
    UnityLogService.hpp
    UnityLogService.cpp
    UnityFileService.hpp
    UnityFileService.cpp
    ${PROTO_SRC}
)

target_link_libraries(MyGameClient
    Game
    ${PROTOBUF_LIBRARY}
    ${PROTOBUF_LITE_LIBRARY}
)

set (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DEXPORT")
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DEXPORT")