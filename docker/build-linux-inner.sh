#!/usr/bin/env bash

set -e

cd /usr/src/myapp

# Generate files
mkdir -p build
rm -rf build/linux64
mkdir -p build/linux64 && cd build/linux64
cmake cmake ../..
cd ../..

rm -f CMakeCache.txt

# Build
cmake --build build/linux64