rd /s/q 3rdparty\build\entt
mkdir 3rdparty\build\entt
pushd 3rdparty\build\entt

cmake -DCMAKE_INSTALL_PREFIX=..\..\ -DCMAKE_GENERATOR_PLATFORM=x64 ..\..\src\entt-3.9.0
cmake --build . --config debug --target install
popd
