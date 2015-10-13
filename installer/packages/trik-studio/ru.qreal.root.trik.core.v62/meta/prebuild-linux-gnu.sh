#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

cp     $BIN_DIR/plugins/tools/kitPlugins/librobots-trik-v62-interpreter.so              $PWD/../data/plugins/tools/kitPlugins/
cp     $BIN_DIR/plugins/tools/kitPlugins/librobots-trik-v62-qts-generator.so            $PWD/../data/plugins/tools/kitPlugins/
cp     $BIN_DIR/plugins/tools/librobots-trik-v62-runtime-uploader-plugin.so             $PWD/../data/plugins/tools/
