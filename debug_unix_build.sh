#!/bin/bash
mkdir -p build
cd build
mkdir -p shaders
cmake -S ../ -B ./ -DCMAKE_BUILD_TYPE=Debug
make && make Shaders
cp ../shaders/* shaders/
lldb -o run -o bt -o quit -- ./Engine
cd ..