add_executable(KcpClient
    main.cpp
)

target_link_libraries(KcpClient
    MyGameClient
)

SET_PROPERTY(TARGET KcpClient PROPERTY FOLDER "Test")