#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

cp     $BIN_DIR/plugins/tools/kitPlugins/librobots-trik-v62-interpreter.dylib              $PWD/../data/plugins/tools/kitPlugins/
cp     $BIN_DIR/plugins/tools/kitPlugins/librobots-trik-v62-qts-generator.dylib            $PWD/../data/plugins/tools/kitPlugins/
