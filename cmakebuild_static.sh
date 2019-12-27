#!/bin/bash
mkdir build
rm -rf build/*
cd build
cmake .. "$@" -DBUILD_SHARED_LIBS:BOOL=OFF   || cmake.exe .. "$@" -DBUILD_SHARED_LIBS:BOOL=OFF 
