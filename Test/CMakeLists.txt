add_executable(Test
    main.cpp
)


SET_PROPERTY(TARGET Test PROPERTY FOLDER "Test")

add_subdirectory(OctTree)
add_subdirectory(Serialize)

#设置VS工作目录
set_target_properties(Test PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/Asset)

