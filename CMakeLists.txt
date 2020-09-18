cmake_minimum_required(VERSION 3.9)
set(CMAKE_CXX_STANDARD 17)
project(MyGame)

#设置文件夹归类
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

#添加目录引用
include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/3rdparty/include")

#define debug
set (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG -D_DEBUG")
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG -D_DEBUG")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin/release)

if(NOT TARGET spdlog)
    # Stand-alone build
    find_package(spdlog REQUIRED)
endif()

add_subdirectory(Framework)
add_subdirectory(Client)
add_subdirectory(Server)
