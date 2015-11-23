#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

mkdir -p $PWD/../data/lib/plugins/editors/
mkdir -p $PWD/../data/lib/plugins/tools/
mkdir -p $PWD/../data/bin/
mkdir -p $PWD/../data/resources/
cp     $BIN_DIR/plugins/editors/librobotsMetamodel.so                   $PWD/../data/lib/plugins/editors/
cp -pr $BIN_DIR/librobots-interpreter-core.so*                          $PWD/../data/lib/
cp     $BIN_DIR/plugins/tools/librobots-plugin.so                       $PWD/../data/lib/plugins/tools/
cp -pr $BIN_DIR/librobots-2d-model.so*                                  $PWD/../data/lib/
cp -pr $BIN_DIR/librobots-generator-base.so*                            $PWD/../data/lib/
cp -pr $BIN_DIR/librobots-kit-base.so*                                  $PWD/../data/lib/
cp -pr $BIN_DIR/librobots-utils.so*                                     $PWD/../data/lib/

# Cleaning up prebuild-common.sh results...
rm -rf $PWD/../data/plugins/
mv $PWD/../data/help $PWD/../data/resources/
mv $PWD/../data/images $PWD/../data/resources/
mv $PWD/../data/splashscreen.png $PWD/../data/bin/
