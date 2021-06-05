
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
    ClientNetwork.hpp
    ClientNetwork.cpp
    ClientNetworkService.hpp
    ClientNetworkService.cpp
)

target_link_libraries(MyGameClient
    Game
    Proto
)

set (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DEXPORT")
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DEXPORT")

set (CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DEXPORT")
set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DEXPORT")