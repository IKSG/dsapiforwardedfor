#!/usr/bin/env bash

set -e

cd "$(dirname "${BASH_SOURCE[0]}")"

mkdir -p build
rm -rf build/macos
mkdir -p build/macos
cd build/macos

cmake ../.. -G Xcode 
