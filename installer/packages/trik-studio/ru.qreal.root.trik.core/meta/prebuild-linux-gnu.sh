#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

cp -pr $BIN_DIR/librobots-trik-qts-generator-library.so*                           $PWD/../data/
cp -pr $BIN_DIR/librobots-trik-generator-base.so*                                  $PWD/../data/
cp -pr $BIN_DIR/librobots-trik-kit.so*                                             $PWD/../data/
cp -pr $BIN_DIR/librobots-trik-kit-interpreter-common.so*                          $PWD/../data/

cp -pr $QT_DIR/lib/libQt5Network.so*                                               $PWD/../data/
