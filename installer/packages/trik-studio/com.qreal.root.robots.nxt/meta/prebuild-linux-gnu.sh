#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"


cp     $BIN_DIR/plugins/tools/kitPlugins/libnxtKitInterpreter.so        $PWD/../data/plugins/tools/kitPlugins/
cp     $BIN_DIR/plugins/tools/kitPlugins/libnxtGenerator.so             $PWD/../data/plugins/tools/kitPlugins/
cp     $BIN_DIR/plugins/tools/kitPlugins/librussianCGenerator.so        $PWD/../data/plugins/tools/kitPlugins/
cp -pr $BIN_DIR/libqextserialport.so*                                   $PWD/../data
