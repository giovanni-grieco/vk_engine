#!/bin/bash
mkdir -p build
cd build
cmake -S ../ -B ./ -DCMAKE_BUILD_TYPE=Debug
make && make Shaders && ./Engine
cd ..