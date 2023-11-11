#!/bin/bash

# Engine Build and Run Script in build/ directory
rm -rf build
cmake -B build
cd build
make
./Engine
