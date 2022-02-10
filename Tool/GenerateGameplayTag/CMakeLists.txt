add_executable(GenerateGameplayTag main.cpp)

target_link_libraries(GenerateGameplayTag lua)
SET_PROPERTY(TARGET GenerateGameplayTag PROPERTY FOLDER "Tool")