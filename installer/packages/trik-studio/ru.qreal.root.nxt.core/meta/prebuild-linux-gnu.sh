#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"


mkdir -p $PWD/../data/lib/plugins/tools/kitPlugins/
cp     $BIN_DIR/plugins/tools/kitPlugins/librobots-nxt-interpreter.so            $PWD/../data/lib/plugins/tools/kitPlugins/
cp     $BIN_DIR/plugins/tools/kitPlugins/librobots-nxt-osek-c-generator.so       $PWD/../data/lib/plugins/tools/kitPlugins/
cp     $BIN_DIR/plugins/tools/kitPlugins/librobots-nxt-russian-c-generator.so    $PWD/../data/lib/plugins/tools/kitPlugins/
cp -pr $BIN_DIR/librobots-nxt-generator-base.so*                                 $PWD/../data/lib
cp -pr $BIN_DIR/librobots-nxt-kit.so*                                            $PWD/../data/lib
cp -pr $BIN_DIR/libqextserialport.so*                                            $PWD/../data/lib

# Cleaning up prebuild-common.sh results...
rm -rf $PWD/../data/plugins

