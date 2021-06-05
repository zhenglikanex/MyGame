#收集proto文件
file(GLOB_RECURSE PROTO_SRC ${PROJECT_SOURCE_DIR}/Framework/Proto/*.h ${PROJECT_SOURCE_DIR}/Framework/Proto/*.cc)

add_library(Proto
    ${PROTO_SRC}
    Kcp/ikcp.h
    Kcp/ikcp.c
)

target_link_libraries(Proto
    debug ${PROTOBUF_LIBRARY_DEBUG}
    debug ${PROTOBUF_LITE_LIBRARY_DEBUG}
    optimized ${PROTOBUF_LIBRARY}
    optimized ${PROTOBUF_LITE_LIBRARY}
)