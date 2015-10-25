#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

mkdir -p $PWD/../data/lib/
mkdir -p $PWD/../data/resources/
cp     $BIN_DIR/librobots-trik-f-sharp-generator-library.so                            $PWD/../data/lib/
mv $PWD/../data/Trik.Core.dll $PWD/../data/resources/
