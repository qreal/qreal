#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

mkdir -p $PWD/../data/plugins/editors
mkdir -p $PWD/../data/plugins/tools
cp -r  $BIN_DIR/help                                                    $PWD/../data
cp -r  $BIN_DIR/images                                                  $PWD/../data
cp     $BIN_DIR/plugins/editors/librobotsMetamodel.so                   $PWD/../data/plugins/editors/
cp     $BIN_DIR/plugins/tools/libinterpreterCore.so                     $PWD/../data/plugins/tools/
cp -pr $BIN_DIR/libcommonTwoDModel.so*                                  $PWD/../data
cp -pr $BIN_DIR/libgeneratorBase.so*                                    $PWD/../data
cp -pr $BIN_DIR/libinterpreterBase.so*                                  $PWD/../data
