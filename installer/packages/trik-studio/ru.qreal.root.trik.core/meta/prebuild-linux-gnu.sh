#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

cp -pr $BIN_DIR/librobots-trik-qts-generator-library.so*                           $PWD/../data/
cp -pr $BIN_DIR/librobots-trik-generator-base.so*                                  $PWD/../data/
cp -pr $BIN_DIR/librobots-trik-kit.so*                                             $PWD/../data/
cp -pr $BIN_DIR/librobots-trik-kit-interpreter-common.so*                          $PWD/../data/

cp -pr $QT_DIR/lib/libQt5Network.so*                                               $PWD/../data/

cd "$(dirname "$0")"/../data
rm -rf winscp
mkdir -p trikRuntime
cd trikRuntime
tar xfvzh ../trik-runtime-builds/trikRuntime.tar.gz
rm -rf ../trik-runtime-builds
