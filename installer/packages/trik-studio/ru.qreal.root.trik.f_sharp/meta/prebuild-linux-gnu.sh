#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

cp     $BIN_DIR/librobots-trik-v62-f-sharp-generator-library.so                            $PWD/../data/
