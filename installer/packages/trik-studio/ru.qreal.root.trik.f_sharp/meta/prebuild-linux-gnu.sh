#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

mkdir -p $PWD/../data/lib/
cp     $BIN_DIR/librobots-trik-f-sharp-generator-library.so                            $PWD/../data/lib/

# Cleaning up prebuild-common.sh results...
mkdir -p $PWD/../data/resources/
mv $PWD/../data/Trik.Core.dll $PWD/../data/resources/
