#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"


cp     $BIN_DIR/plugins/tools/kitPlugins/librobots-nxt-interpreter.so        $PWD/../data/plugins/tools/kitPlugins/
cp     $BIN_DIR/plugins/tools/kitPlugins/librobots-nxt-osek-c-generator.so   $PWD/../data/plugins/tools/kitPlugins/
cp     $BIN_DIR/plugins/tools/kitPlugins/librobots-russian-c-generator.so    $PWD/../data/plugins/tools/kitPlugins/
cp -pr $BIN_DIR/librobots-nxt-generator-base.so*                             $PWD/../data
cp -pr $BIN_DIR/libqextserialport.so*                                        $PWD/../data
