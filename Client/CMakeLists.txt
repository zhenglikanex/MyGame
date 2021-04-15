
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
    UnityDebugService.hpp
    UnityDebugService.cpp
    UnityFileService.hpp
    UnityFileService.cpp
    ClientNetworkService.hpp
    ClientNetworkService.cpp
    ${PROTO_SRC}
)

target_link_libraries(MyGameClient
    Game
    debug ${PROTOBUF_LIBRARY_DEBUG}
    debug ${PROTOBUF_LITE_LIBRARY_DEBUG}
    optimized ${PROTOBUF_LIBRARY}
    optimized ${PROTOBUF_LITE_LIBRARY}
)

set (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DEXPORT")
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DEXPORT")

set (CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DEXPORT")
set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DEXPORT")