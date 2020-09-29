
add_library(MyGameClient SHARED
    main.cpp
    UnityBridge.hpp
    UnityBridge.cpp
    UnityViewImpl.hpp
    UnityViewImpl.cpp
    UnityViewService.hpp
    UnityViewService.cpp
)

target_link_libraries(MyGameClient
    Game
)

