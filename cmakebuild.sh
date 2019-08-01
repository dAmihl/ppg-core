#!/bin/bash
mkdir build
rm -rf build/*
cd build
cmake .. "$@"  || cmake.exe .. "$@" 
