mkdir build
pushd build

cmake -D CMAKE_PREFIX_PATH=3rdparty/lib/cmake -Thost=x64 ..\.
cmake --build . --config debug --target install
popd
