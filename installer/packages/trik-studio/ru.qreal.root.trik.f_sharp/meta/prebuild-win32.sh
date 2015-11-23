#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

cp    $BIN_DIR/robots-trik-f-sharp-generator-library.dll                              $PWD/../data/
