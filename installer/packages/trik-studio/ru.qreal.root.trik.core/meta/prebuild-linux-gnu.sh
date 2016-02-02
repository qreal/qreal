#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

mkdir -p $PWD/../data/lib
cp -pr $BIN_DIR/librobots-trik-qts-generator-library.so*                           $PWD/../data/lib/
cp -pr $BIN_DIR/librobots-trik-generator-base.so*                                  $PWD/../data/lib/
cp -pr $BIN_DIR/librobots-trik-kit.so*                                             $PWD/../data/lib/
cp -pr $BIN_DIR/librobots-trik-kit-interpreter-common.so*                          $PWD/../data/lib/

cp -pr $QT_DIR/lib/libQt5Network.so*                                               $PWD/../data/lib/

cd "$(dirname "$0")"/../data
rm -rf winscp
mkdir -p resources/trikRuntime
cd resources/trikRuntime
tar xfvzh ../../trik-runtime-builds/trikRuntime.tar.gz
rm -rf ../../trik-runtime-builds
