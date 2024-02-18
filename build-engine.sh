#!/bin/bash

# Engine Build and Run Script in build/ directory

# rm -rf build
if [[ $* == *--clean* ]]
  then
    echo "cleaning build directory..."
    rm -rf build 
fi

# BUILD OUT CMAKE ARGS
cmake_args="-B build"


if [[ $* == *--editor* ]]
  then
    cmake_args="-DEDITOR=ON ${cmake_args}"
elif [[ $* == *--debug* ]]
  then
    cmake_args="-DCMAKE_BUILD_TYPE=Debug ${cmake_args}"
fi

if [[ $* == *--verbose* ]]
  then
    cmake_args="-DDEBUG_MESSAGES=ON ${cmake_args}"
fi

# BUILD WITH CMAKE & CUSTOM ARGS
# echo $cmake_args
cmake $cmake_args
cd build

# RUN NINJA IF NEEDED
if [[ $* == *--ninja* ]]
  then
    ninja
else
  make
fi

# EXECUTE
./Engine
