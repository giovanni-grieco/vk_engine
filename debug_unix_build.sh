#!/bin/bash
mkdir -p build
cd build
cmake -S ../ -B ./ -DCMAKE_BUILD_TYPE=Debug
make && make Shaders
mkdir -p shaders
mkdir -p textures
mkdir -p models
cp ../shaders/* shaders/
cp ../textures/* textures/
cp ../models/* models/
./Engine
cd ..