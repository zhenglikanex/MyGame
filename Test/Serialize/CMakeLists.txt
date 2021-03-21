add_executable(Serialize
  main.cpp
)

target_link_libraries(Serialize Game)
SET_PROPERTY(TARGET Serialize PROPERTY FOLDER "Test")