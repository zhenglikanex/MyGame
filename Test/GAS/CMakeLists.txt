add_executable(GAS main.cpp)

target_link_libraries(GAS lua)
SET_PROPERTY(TARGET GAS PROPERTY FOLDER "Test")

set_target_properties(GAS PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/Asset)