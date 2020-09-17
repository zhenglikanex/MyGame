
add_library(MyGameClient SHARED
    main.cpp
)

target_link_libraries(MyGameClient
    Game
)

