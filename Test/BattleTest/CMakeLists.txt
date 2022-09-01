add_executable(BattleTest
    main.cpp
)

target_link_libraries(BattleTest
    BattleActor
)

SET_PROPERTY(TARGET BattleTest PROPERTY FOLDER "Test")

#设置VS工作目录
set_target_properties(BattleTest PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/Asset)