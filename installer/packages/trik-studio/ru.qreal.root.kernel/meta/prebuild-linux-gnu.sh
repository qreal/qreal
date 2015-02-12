#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

cp     $BIN_DIR/plugins/editors/librobotsMetamodel.so                   $PWD/../data/plugins/editors/
cp -pr $BIN_DIR/librobots-interpreter-core.so*                          $PWD/../data
cp     $BIN_DIR/plugins/tools/librobots-plugin.so                       $PWD/../data/plugins/tools/
cp -pr $BIN_DIR/librobots-common-2d-model.so*                           $PWD/../data
cp -pr $BIN_DIR/librobots-generator-base.so*                            $PWD/../data
cp -pr $BIN_DIR/librobots-interpreter-base.so*                          $PWD/../data
cp -pr $BIN_DIR/librobots-utils.so*                                     $PWD/../data
