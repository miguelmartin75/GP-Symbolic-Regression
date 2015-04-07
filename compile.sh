#!/bin/bash

(
cd $1
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
)
