cmake_minimum_required(VERSION 3.9)
set(CMAKE_CXX_STANDARD 17)
project(MyGame)

#设置文件夹归类
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_PREFIX_PATH "${PROJECT_SOURCE_DIR}/3rdparty/lib/cmake")

#添加目录引用
include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/3rdparty/include")

#define debug
set (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG -D_DEBUG")
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG -D_DEBUG")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin/release)
message(${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG})

set(PROTOBUF_LIB_PATH ${PROJECT_SOURCE_DIR}/3rdparty/lib/)
message(${PROTOBUF_LIB_PATH})

#寻找protobuf
find_library(PROTOBUF_LIBRARY NAMES libprotobuf libprotobufd PATHS ${PROTOBUF_LIB_PATH})
find_library(PROTOBUF_LITE_LIBRARY NAMES libprotobuf-lite libprotobuf-lited PATHS ${PROTOBUF_LIB_PATH})

#收集proto文件
file(GLOB_RECURSE PROTO_SRC ${PROJECT_SOURCE_DIR}/Framework/Proto/*.h ${PROJECT_SOURCE_DIR}/Framework/Proto/*.cc)

add_subdirectory(Framework)
add_subdirectory(Client)
add_subdirectory(Server)
add_subdirectory(Test)
