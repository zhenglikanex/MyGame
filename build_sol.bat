rd /s/q 3rdparty\build\sol2
mkdir 3rdparty\build\sol2
pushd 3rdparty\build\sol2
cmake -DCMAKE_INSTALL_PREFIX=..\..\ -DCMAKE_GENERATOR_PLATFORM=x64 ..\..\src\sol2-3.2.3
cmake --build . --config debug --target install
cmake --build . --config release --target install