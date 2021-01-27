add_executable(OctTree
main.cpp
OctTree.hpp
OctTreeNode.hpp
Primitive.hpp
Collision.hpp
ECSOctTree.hpp
)

SET_PROPERTY(TARGET OctTree PROPERTY FOLDER "Test")