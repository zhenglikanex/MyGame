add_executable(Test
    main.cpp
)

target_link_libraries(Test Proto)

SET_PROPERTY(TARGET Test PROPERTY FOLDER "Test")

add_subdirectory(OctTree)
add_subdirectory(Serialize)
add_subdirectory(AsioClient)
add_subdirectory(AsioServer)
#add_subdirectory(KcpClient)
add_subdirectory(PhysXTest)

#设置VS工作目录
set_target_properties(Test PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/Asset)

