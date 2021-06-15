rd /s/q 3rdparty\build\fpm
mkdir 3rdparty\build\fpm
pushd 3rdparty\build\fpm
cmake -DCMAKE_INSTALL_PREFIX=..\..\ -DCMAKE_GENERATOR_PLATFORM=x64 ..\..\src\fpm-1.0.1
cmake --build . --config debug --target install
popd
