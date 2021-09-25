add_executable(Sol main.cpp)

target_link_libraries(Sol lua)
SET_PROPERTY(TARGET Sol PROPERTY FOLDER "Test")