#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"


mkdir -p $PWD/../data/lib/plugins/tools/kitPlugins
cp     $BIN_DIR/plugins/tools/kitPlugins/librobots-ev3-interpreter.so   $PWD/../data/lib/plugins/tools/kitPlugins/
cp -pr $BIN_DIR/librobots-ev3-kit.so*                                   $PWD/../data/lib
cp -pr $BIN_DIR/libqextserialport.so*                                   $PWD/../data/lib

# Cleaning up prebuild-common.sh results...
rm -rf $PWD/../data/plugins/
mv $PWD/../data/examples $PWD/../data/resources
