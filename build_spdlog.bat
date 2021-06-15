rd /s/q 3rdparty\build\spdlog
mkdir 3rdparty\build\spdlog
pushd 3rdparty\build\spdlog
cmake -DCMAKE_INSTALL_PREFIX=..\..\ -DCMAKE_GENERATOR_PLATFORM=x64 ..\..\src\spdlog-1.7.0
cmake --build . --config debug --target install
popd
