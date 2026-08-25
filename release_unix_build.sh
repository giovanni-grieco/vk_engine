#!/bin/bash
mkdir -p build
cd build
cmake -S ../ -B ./ -DCMAKE_BUILD_TYPE=Release
make && make Shaders
mkdir -p shaders
mkdir -p textures
cp ../shaders/* shaders/
cp ../textures/* textures/
./Engine
cd ..