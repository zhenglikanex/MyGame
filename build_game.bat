rd /s/q build
mkdir build
pushd build
cmake -DCMAKE_INSTALL_PREFIX=. -DCMAKE_GENERATOR_PLATFORM=x64 ..\.
cmake --build . --config debug --target install
