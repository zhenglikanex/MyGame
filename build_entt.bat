mkdir 3rdparty\build\entt
pushd 3rdparty\build\entt
rm -rf *
cmake -DCMAKE_INSTALL_PREFIX=..\..\include -G "Visual Studio 15 2017 Win64" -Thost=x64 ..\..\src\entt-3.5.1
cmake --build . --config debug --target install
popd
