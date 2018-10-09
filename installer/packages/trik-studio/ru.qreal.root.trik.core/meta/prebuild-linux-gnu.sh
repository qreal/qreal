#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

mkdir -p $PWD/../data/lib
mkdir -p $PWD/../data/lib/plugins/editors
cp     $BIN_DIR/plugins/editors/libtrikMetamodel.so                                $PWD/../data/lib/plugins/editors
cp -pr $BIN_DIR/librobots-trik-qts-generator-library.so*                           $PWD/../data/lib/
cp -pr $BIN_DIR/librobots-trik-generator-base.so*                                  $PWD/../data/lib/
cp -pr $BIN_DIR/librobots-trik-kit.so*                                             $PWD/../data/lib/
cp -pr $BIN_DIR/librobots-trik-kit-interpreter-common.so*                          $PWD/../data/lib/
cp -pr $BIN_DIR/libPythonQt-Qt*-Python*.so*                                        $PWD/../data/lib/
cp -pr $BIN_DIR/libtrik*.so*                                                         $PWD/../data/lib/

cp     $BIN_DIR/system.{py,js}                                                		 $PWD/../data/bin/

cd "$(dirname "$0")"/../data
rm -rf winscp
mkdir -p resources/trikRuntime
cd resources/trikRuntime
tar xfvz ../../trik-runtime-builds/trikRuntime.tar.gz
rm -rf ../../trik-runtime-builds
