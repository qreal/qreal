#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

mkdir -p $PWD/../data/lib/plugins/tools/kitPlugins
cp     $BIN_DIR/plugins/tools/kitPlugins/librobots-trik-v6-interpreter.so              $PWD/../data/lib/plugins/tools/kitPlugins/
cp     $BIN_DIR/plugins/tools/kitPlugins/librobots-trik-v6-qts-generator.so            $PWD/../data/lib/plugins/tools/kitPlugins/
cp     $BIN_DIR/plugins/tools/librobots-trik-v6-runtime-uploader-plugin.so             $PWD/../data/lib/plugins/tools/

# Cleaning up prebuild-common.sh results...
rm -rf $PWD/../data/plugins/
