rd /s/q 3rdparty\build\protobuf
mkdir 3rdparty\build\protobuf
pushd 3rdparty\build\protobuf
cmake -DCMAKE_INSTALL_PREFIX=..\..\ -G "Visual Studio 16 2019" ..\..\src\protobuf-3.0.0\cmake
cmake --build . --config debug --target install
popd
pushd 3rdparty
call build\protobuf\extract_includes.bat
popd