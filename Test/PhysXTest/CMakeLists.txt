add_executable(PhysXTest
  main.cpp
)

target_link_libraries(PhysXTest
    debug ${PHYSX_LIBRARY_DEBU0G}
    debug ${PHYSX_COMMON_LIBRARY_DEBU0G}
    debug ${PX_FOUNDATION_LIBARY_DEBUG}
    debug ${PHYSX_COOKING_LIBRARY_DEBUG}
    debug ${PX_PVD_LIBARY_DEBUG}
    debug ${PHYSX_EXTENSIONS_LIBRARY_DEBU0G}

    optimized ${PHYSX_LIBRARY}
    optimized ${PHYSX_COMMON_LIBRARY}
    optimized ${PX_FOUNDATION_LIBARY}
    optimized ${PHYSX_COOKING_LIBRARY}
    optimized ${PX_PVD_LIBARY}
    optimized ${PHYSX_EXTENSIONS_LIBRARY}
)

SET_PROPERTY(TARGET PhysXTest PROPERTY FOLDER "Test")