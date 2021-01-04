add_executable(Test
    main.cpp
)

target_link_libraries(Test
    Game
)

#设置VS工作目录
set_target_properties(ActorNet PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/Asset)