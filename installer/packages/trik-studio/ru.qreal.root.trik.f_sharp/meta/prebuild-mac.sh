#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

cp     $BIN_DIR/librobots-trik-f-sharp-generator-library.dylib                            $PWD/../data/
