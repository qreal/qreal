#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"


cp     $BIN_DIR/plugins/tools/kitPlugins/librobots-nxt-interpreter.dylib            $PWD/../data/plugins/tools/kitPlugins/
cp     $BIN_DIR/plugins/tools/kitPlugins/librobots-nxt-osek-c-generator.dylib       $PWD/../data/plugins/tools/kitPlugins/
cp     $BIN_DIR/plugins/tools/kitPlugins/librobots-nxt-russian-c-generator.dylib    $PWD/../data/plugins/tools/kitPlugins/
cp -pr $BIN_DIR/librobots-nxt-generator-base*.dylib                                 $PWD/../data
cp -pr $BIN_DIR/librobots-nxt-kit*.dylib                                            $PWD/../data
cp -pr $BIN_DIR/libqextserialport*.dylib                                            $PWD/../data
