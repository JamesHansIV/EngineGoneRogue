#!/bin/bash

# Engine Build and Run Script in build/ directory

rm -rf build
if [ $1 == "editor" ]
  then
    cmake -DEDITOR=ON -B build
else
  cmake -B build
fi
cd build
make
./Engine
