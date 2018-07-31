#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

mkdir -p $PWD/../data/lib/
mkdir -p $PWD/../data/lib/plugins/tools/kitPlugins/

cp    $BIN_DIR/librobots-trik-python-generator-library.so                              $PWD/../data/lib/
cp    $BIN_DIR/plugins/tools/kitPlugins/librobots-trik-v62-python-generator.so            $PWD/../data/lib/plugins/tools/kitPlugins/

