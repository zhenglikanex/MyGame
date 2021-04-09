add_executable(ServerApp
    main.cpp
)

target_link_libraries(ServerApp
    ActorNet
)

SET_PROPERTY(TARGET ServerApp PROPERTY FOLDER "Server")

#设置VS工作目录
set_target_properties(ServerApp PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/Asset)