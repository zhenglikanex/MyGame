cmake_minimum_required(VERSION 3.9)
set(CMAKE_CXX_STANDARD 17)
project(Kanex)

#添加camke模块
include(cmake/common.cmake)

#设置文件夹归类
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

include_directories("${PROJECT_SOURCE_DIR}/Src")

add_subdirectory(Src)
add_subdirectory(Test)