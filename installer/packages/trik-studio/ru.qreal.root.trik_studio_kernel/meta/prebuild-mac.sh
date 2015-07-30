#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

cp     $BIN_DIR/plugins/editors/librobotsMetamodel.dylib                   $PWD/../data/plugins/editors/
cp -pr $BIN_DIR/librobots-interpreter-core*.dylib                          $PWD/../data
cp     $BIN_DIR/plugins/tools/librobots-plugin.dylib                       $PWD/../data/plugins/tools/
cp -pr $BIN_DIR/librobots-2d-model*.dylib                                  $PWD/../data
cp -pr $BIN_DIR/librobots-generator-base*.dylib                            $PWD/../data
cp -pr $BIN_DIR/librobots-kit-base*.dylib                                  $PWD/../data
cp -pr $BIN_DIR/librobots-utils*.dylib                                     $PWD/../data
