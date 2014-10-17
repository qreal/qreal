#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"


cp     $BIN_DIR/plugins/tools/kitPlugins/libev3KitInterpreter.so        $PWD/../data/plugins/tools/kitPlugins/
cp -pr $BIN_DIR/libqextserialport.so*                                   $PWD/../data
