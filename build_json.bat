rd /s/q 3rdparty\build\json
mkdir 3rdparty\build\json
pushd 3rdparty\build\json
cmake -DCMAKE_INSTALL_PREFIX=..\..\ -Thost=x64 ..\..\src\json-3.9.1
cmake --build .
popd
