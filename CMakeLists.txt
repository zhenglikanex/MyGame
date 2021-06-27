cmake_minimum_required(VERSION 3.9)
set(CMAKE_CXX_STANDARD 17)
project(MyGame)

#添加camke模块
include(cmake/common.cmake)

#设置文件夹归�?
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

if(WIN32)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
endif()

set(CMAKE_PREFIX_PATH "${PROJECT_SOURCE_DIR}/3rdparty/lib/cmake")
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}" CACHE PATH "${PROJECT_SOURCE_DIR}" FORCE)
endif()

#添加目录引用
include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/3rdparty/include")
include_directories("${PROJECT_SOURCE_DIR}/3rdparty/include/physx")
include_directories("${PROJECT_SOURCE_DIR}/3rdparty/include/pxshared")
include_directories("${PROJECT_SOURCE_DIR}/Framework/Kanex/Src")

#define debug
set (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG -D_DEBUG")
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG -D_DEBUG")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin/release)

set(PROTOBUF_LIB_PATH ${PROJECT_SOURCE_DIR}/3rdparty/lib/)
set(PHYSX_LIB_PATH ${PROJECT_SOURCE_DIR}/3rdparty/lib/physx)
set(PX_LIB_PATH ${PROJECT_SOURCE_DIR}/3rdparty/lib/pxshared)
message(${PROTOBUF_LIB_PATH})

#寻找protobuf
find_library(PROTOBUF_LIBRARY_DEBUG NAMES libprotobufd PATHS ${PROTOBUF_LIB_PATH})
find_library(PROTOBUF_LITE_LIBRARY_DEBUG NAMES libprotobuf-lited PATHS ${PROTOBUF_LIB_PATH})
find_library(PROTOBUF_LIBRARY NAMES libprotobuf PATHS ${PROTOBUF_LIB_PATH})
find_library(PROTOBUF_LITE_LIBRARY NAMES libprotobuf-lite PATHS ${PROTOBUF_LIB_PATH})

find_library(PHYSX_LIBRARY_DEBU0G NAMES PhysX3DEBUG_x64 PATHS ${PHYSX_LIB_PATH})
find_library(PHYSX_COMMON_LIBRARY_DEBU0G NAMES PhysX3CommonDEBUG_x64 PATHS ${PHYSX_LIB_PATH})
find_library(PX_FOUNDATION_LIBARY_DEBUG NAMES PxFoundationDEBUG_x64 PATHS ${PX_LIB_PATH})
find_library(PHYSX_COOKING_LIBRARY_DEBUG NAMES PhysX3CookingDEBUG_x64 PATHS ${PHYSX_LIB_PATH})
find_library(PHYSX_EXTENSIONS_LIBRARY_DEBU0G NAMES PhysX3ExtensionsDEBUG PATHS ${PHYSX_LIB_PATH})
find_library(PX_PVD_LIBARY_DEBUG NAMES PxPvdSDKDEBUG_x64 PATHS ${PX_LIB_PATH})

find_library(PHYSX_LIBRARY NAMES PhysX3_x64 PATHS ${PHYSX_LIB_PATH})
find_library(PHYSX_COMMON_LIBRARY NAMES PhysX3Common_x64 PATHS ${PHYSX_LIB_PATH})
find_library(PX_FOUNDATION_LIBARY NAMES PxFoundation_x64 PATHS ${PX_LIB_PATH})
find_library(PHYSX_COOKING_LIBRARY NAMES PhysX3Cooking_x64 PATHS ${PHYSX_LIB_PATH})
find_library(PHYSX_EXTENSIONS_LIBRARY NAMES PhysX3Extensions PATHS ${PHYSX_LIB_PATH})
find_library(PX_PVD_LIBARY NAMES PxPvdSDK_x64 PATHS ${PX_LIB_PATH})

add_subdirectory(Framework)
add_subdirectory(Client)
add_subdirectory(Server)
add_subdirectory(Test)
add_subdirectory(ClientProxy)

#安装MyGameClient到Unity Plugins目录�?
install(TARGETS MyGameClient
    RUNTIME DESTINATION Unity/ARPG
    LIBRARY DESTINATION Unity/ARPG
    )
install(FILES ${PROJECT_SOURCE_DIR}/bin/debug/MyGameClient.pdb DESTINATION Unity/ARPG)
install(FILES ${PROJECT_SOURCE_DIR}/bin/debug/MyGameClient.pdb DESTINATION UnityApp)
install(FILES Unity/ARPG/MyGameClient.dll DESTINATION UnityApp)

install(TARGETS ClientProxy
RUNTIME DESTINATION Unity/ARPG/Assets/Plugins
LIBRARY DESTINATION Unity/ARPG/Assets/Plugins
)
install(FILES ${PROJECT_SOURCE_DIR}/bin/debug/ClientProxy.pdb DESTINATION Unity/ARPG/Assets/Plugins)