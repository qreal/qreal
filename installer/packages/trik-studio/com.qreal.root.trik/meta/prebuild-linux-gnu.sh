#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

cp     $BIN_DIR/plugins/tools/kitPlugins/librobots-trik-interpreter.so                 $PWD/../data/plugins/tools/kitPlugins/
cp     $BIN_DIR/plugins/tools/kitPlugins/librobots-trik-qts-generator.so               $PWD/../data/plugins/tools/kitPlugins/
cp     $BIN_DIR/plugins/tools/kitPlugins/librobots-trik-runtime-uploader-plugin.so     $PWD/../data/plugins/tools/kitPlugins/
cp -pr $BIN_DIR/librobots-trik-generator-base.so*                                      $PWD/../data

cp -pr $QT_DIR/lib/libQt5Network.so*                                                   $PWD/../data
