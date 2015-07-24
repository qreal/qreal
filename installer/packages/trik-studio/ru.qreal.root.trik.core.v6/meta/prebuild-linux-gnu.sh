#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

cp     $BIN_DIR/plugins/tools/kitPlugins/librobots-trik-v6-interpreter.so              $PWD/../data/plugins/tools/kitPlugins/
cp     $BIN_DIR/plugins/tools/kitPlugins/librobots-trik-v6-qts-generator.so            $PWD/../data/plugins/tools/kitPlugins/
