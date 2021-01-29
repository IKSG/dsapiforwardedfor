#!/usr/bin/env bash

set -e;

echo "Configuring for Xcode"
mkdir -p build
rm -rf build/xcode
mkdir -p build/xcode && cd build/xcode
cmake .. -G Xcode