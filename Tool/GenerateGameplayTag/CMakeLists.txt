add_executable(GenerateGameplayTag main.cpp)
SET_PROPERTY(TARGET GenerateGameplayTag PROPERTY FOLDER "Tool")
set_target_properties(GenerateGameplayTag PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})