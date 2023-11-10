#!/bin/bash

# Engine Build and Run Script in build/ directory
cmake -B build
cd build
make
./Engine
